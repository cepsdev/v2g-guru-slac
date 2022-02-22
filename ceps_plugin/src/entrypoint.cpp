/*
MIT License

Copyright (c) 2021 Tomas Prerovsky

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <stdlib.h>
#include <iostream>
#include <ctype.h>
#include <chrono>
#include <sstream>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <queue>
#include <unordered_map>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <stdlib.h>
#include <map>
#include <algorithm>
#include <future>
#include <netinet/sctp.h> 

#include "mme4ceps.hpp"
#include "mme2ceps_utils.hpp"
#include "ceps2mme.hpp"
#include "tests/test_handle_homeplug_mme.hpp"

static Ism4ceps_plugin_interface* plugin_master = nullptr;
static mme4ceps_plugin plugn;



static ceps::ast::node_t plugin_entrypoint_route_mme(ceps::ast::node_callparameters_t params){
    auto eval_ceps_handler = [&](std::vector<ceps::ast::node_t> v){
      for(auto e: v){
        std::string sym_name;
        std::string kind_name; 
        ceps::ast::nodes_t args;
        if (is_a_symbol_with_arguments(e,sym_name,kind_name,args)){
            if (kind_name == "Event") plugn.ceps_engine->queue_internal_event(sym_name,args);
        } else if (ceps::ast::is_a_symbol(e)){
          auto& sym = ceps::ast::as_symbol_ref(e);
          if (ceps::ast::kind(sym) == "Event") plugn.ceps_engine->queue_event(ceps::ast::name(sym),{}); 
        }
      }
    };
    auto t = get_first_child(params);
    ceps::ast::node_t ceps_script_clone{};
    if (t) plugn.set_associated_ceps_block(ceps_script_clone = t->clone());
    auto on_initplugin =  ceps::ast::Nodeset{t}["setup"]["on_initplugin"].nodes();
    eval_ceps_handler(on_initplugin);

    //start_sctp_server
    auto comm = ceps::ast::Nodeset{t}["setup"]["communication"];
    auto err_ev_ns = ceps::ast::Nodeset{t}["setup"]["on_error"].nodes();
    auto on_new_client = ceps::ast::Nodeset{ceps_script_clone}["setup"]["on_new_client"].nodes();
    plugn.on_client_connect = on_new_client;

    {
      // exclude/include vlan_tag 
      auto encode_vlan_tag = ceps::ast::Nodeset{ceps_script_clone}["setup"]["encode_vlan_tag"];
      auto r = encode_vlan_tag.as_int_noexcept();
      if (r.has_value())
       plugn.encode_vlan_tag = r.value();
    }

    {
      // homeplug_mme_big_endianness 
      auto encode_vlan_tag = ceps::ast::Nodeset{ceps_script_clone}["setup"]["big_endianness"];
      auto r = encode_vlan_tag.as_int_noexcept();
      if (r.has_value())
       plugn.encode_big_endianness = r.value() != 0;
    }    
  
    std::string err_ev;
    if (err_ev_ns.size() == 1 && ceps::ast::is_a_symbol(err_ev_ns[0]))
     err_ev = ceps::ast::name(ceps::ast::as_symbol_ref(err_ev_ns[0]));
    if (comm["as_server"].nodes().size() == 1 && comm["as_server"].as_int_noexcept() != 0){
      std::string port = "56800";
      if(comm["port"].nodes().size() == 1) 
        port = comm["port"].as_str();
      auto result = plugn.start_sctp_server(port.size() == 0 ? "56800" : port);
      if (!result.ok) {
        if (err_ev.size()) 
          plugn.ceps_engine->queue_internal_event(err_ev,{ceps::ast::mk_string(result.msg)});
      }
    } else if (comm["as_server"].nodes().size() == 1 && comm["as_server"].as_int_noexcept() == 0){
      std::string port = "56800";
      if(comm["port"].nodes().size() == 1) 
        port = comm["port"].as_str().size() ? comm["port"].as_str() : port ;
      std::string host = "localhost";
      if(comm["host"].nodes().size() == 1) 
        host = comm["host"].as_str();

      addrinfo hints = {0}, *addrinfo_res = nullptr; memset(&hints,0,sizeof(hints));

      hints.ai_flags = AI_NUMERICSERV;
      hints.ai_family = AF_INET;
      hints.ai_socktype = SOCK_SEQPACKET;
      hints.ai_protocol = IPPROTO_SCTP;

      auto sys_call_result = 0;
      if ((sys_call_result = getaddrinfo(host.c_str(), port.c_str(), &hints,&addrinfo_res)) != 0 )
      {
        if (err_ev.size()) 
          plugn.ceps_engine->queue_internal_event(err_ev,{ceps::ast::mk_string("getaddrinfo failed.")});
        return nullptr;
      }
    
      auto connectfd = socket(AF_INET, SOCK_SEQPACKET, IPPROTO_SCTP);

      if (connectfd < 0)
      {
        if (err_ev.size()) 
          plugn.ceps_engine->queue_internal_event(err_ev,{ceps::ast::mk_string("socket() failed.")});
        return nullptr;
      }

      auto it = addrinfo_res;
      addrinfo* res;
      for(; it != nullptr ; it = it->ai_next){
          res = it;
          break;
      }//for

      if (it == nullptr)
      {
        if (err_ev.size()) 
          plugn.ceps_engine->queue_internal_event(err_ev,{ceps::ast::mk_string("No addrinfo found.")});
        return nullptr;
      }

      sctp_event_subscribe evnts {0};memset(&evnts,0,sizeof(evnts));
      evnts.sctp_data_io_event = 1;
      setsockopt(connectfd, IPPROTO_SCTP, SCTP_EVENTS, &evnts, sizeof(evnts));
      {
        std::lock_guard g{plugn.commfd_mtx};
        plugn.commfd = connectfd;
        plugn.last_client = *(sockaddr_in*)res->ai_addr;
        plugn.last_client_len = res->ai_addrlen;
      }
        std::thread reader_thread {
        [=]()
        {
            //on_exit_cleanup on_exit{ [&] {if (addrinfo_res != nullptr) freeaddrinfo(addrinfo_res);}};
            socklen_t len = it->ai_addrlen;
            char readbuf[2048];
            for(;;){
                sockaddr_in client {0};
                len = sizeof(sockaddr_in);
                sctp_sndrcvinfo sndrcvinfo{0};
                memset(&client,0,sizeof(client));
                memset(&sndrcvinfo,0,sizeof(sndrcvinfo));
                int msg_flags{}; 
                auto rd_sz = sctp_recvmsg(plugn.commfd,readbuf,sizeof(readbuf),(sockaddr*)&client,&len,&sndrcvinfo,&msg_flags);
                if (rd_sz > 0)
                {
                    {
                        std::lock_guard g{plugn.commfd_mtx};
                        plugn.last_client = client;
                        plugn.last_client_len = len;
                    } 
                    plugn.handle_homeplug_mme( (homeplug_mme_generic*) readbuf,rd_sz);
                }
            }
        }
    };

    reader_thread.detach();

    }
        
    if (ceps::ast::Nodeset{t}["setup"]["run_tests"].nodes().size()){
      auto on_start_tests =  ceps::ast::Nodeset{t}["setup"]["on_start_tests"].nodes();
      auto on_end_tests =  ceps::ast::Nodeset{t}["setup"]["on_end_tests"].nodes();
      eval_ceps_handler(on_start_tests);
      tests::handle_homeplug_mme::run_all(plugn);
      eval_ceps_handler(on_end_tests);
    }
    return nullptr;
}

static void write_mme_header(ceps::ast::node_t msg, homeplug_mme_generic& mme_msg, bool encode_big_endian){
  auto ns = ceps::ast::Nodeset{msg}["mme"];
  auto header = ns["header"];
  auto mmtype = header["mmtype"];    
  auto fmi = header["fmi"].as_int_noexcept();
  auto fmsn = header["fmsn"].as_int_noexcept();
  auto mmv = header["mmv"].as_int_noexcept();
  auto mtype = header["mtype"].as_int_noexcept();
  auto oda = header["oda"];
  auto osa = header["osa"];
  auto vlan_tag = header["vlan_tag"].as_int_noexcept();
  auto payload = ns["payload"];
  auto mme_type = mmtype.as_int_noexcept();

  //This is not the right way, @TODO Improve readability, conciseness.

  if (encode_big_endian){
   mme_msg.mmtype = cov_endianness_narrow<Endianness::big,decltype(mme_msg.mmtype),int>(mme_type.value());
   if(vlan_tag.has_value()) mme_msg.vlan_tag = cov_endianness_narrow<Endianness::big,decltype(mme_msg.vlan_tag),int>(vlan_tag.value());
   if(mtype.has_value()) mme_msg.mtype = cov_endianness_narrow<Endianness::big,decltype(mme_msg.mtype),int>(mtype.value());
  } else {
   mme_msg.mmtype = cov_endianness_narrow<Endianness::machine,decltype(mme_msg.mmtype),int>(mme_type.value());
   if(vlan_tag.has_value()) mme_msg.vlan_tag = cov_endianness_narrow<Endianness::machine,decltype(mme_msg.vlan_tag),int>(vlan_tag.value());
   if(mtype.has_value()) mme_msg.mtype = cov_endianness_narrow<Endianness::machine,decltype(mme_msg.mtype),int>(mtype.value());
  }

  
  if(fmi.has_value()) mme_msg.fmi = fmi.value();
  if(fmsn.has_value()) mme_msg.fmsn = fmsn.value();
  if(mmv.has_value()) mme_msg.mmv = mmv.value();
  if(!osa.empty()) write_bytes(osa.nodes(), ((uint8_t*) &mme_msg.osa), ((uint8_t*) &mme_msg.osa) + sizeof(mme_msg.osa));
  if(!oda.empty()) write_bytes(oda.nodes(), ((uint8_t*) &mme_msg.oda), ((uint8_t*) &mme_msg.oda) + sizeof(mme_msg.oda));
}

static ceps::ast::node_t plugin_send_mme(ceps::ast::node_callparameters_t params){
    auto msg = get_first_child(params);
    auto ns = ceps::ast::Nodeset{msg}["mme"];
    auto header = ns["header"];
    auto mmtype_ns = header["mmtype"];    
    auto payload = ns["payload"];
    auto mmtype = mmtype_ns.as_int_noexcept();
    auto print_debug_ns = ns["debug"]; 
    auto print_debug = print_debug_ns.size() > 0 && 
                       ceps::ast::is<ceps::ast::Ast_node_kind::int_literal>(print_debug_ns.nodes()[0]) && 
                       0 != ceps::ast::value(ceps::ast::as_int_ref(print_debug_ns.nodes()[0]));
        
    if (!mmtype.has_value()) return nullptr;
    char mme_msg_buffer[mme::max_frame_size] = {0};
    homeplug_mme_generic& mme_msg = *((homeplug_mme_generic*)mme_msg_buffer);
    
  
    
    write_mme_header(msg, mme_msg, plugn.encode_big_endianness);

    auto payload_size = sizeof(homeplug_mme_generic) - sizeof(homeplug_mme_generic_header);
    auto payload_bytes_written = 0;

    if (mmtype == mme::CM_SLAC_PARM_REQ)
     payload_bytes_written = write(payload.nodes(), mme_msg.mmdata.cm_slac_parm_req, payload_size, plugn.encode_big_endianness ? Endianness::big : Endianness:: machine);
    else if (mmtype == mme::CM_SLAC_PARM_CNF)
     payload_bytes_written = write(payload.nodes(), mme_msg.mmdata.cm_slac_parm_cnf, payload_size, plugn.encode_big_endianness ? Endianness::big : Endianness:: machine);
    else if (mmtype == mme::CM_START_ATTEN_CHAR_IND)
     payload_bytes_written = write(payload.nodes(), mme_msg.mmdata.cm_start_atten_char_ind, payload_size, plugn.encode_big_endianness ? Endianness::big : Endianness:: machine);
    else if (mmtype == mme::CM_MNBC_SOUND_IND)
     payload_bytes_written = write(payload.nodes(), mme_msg.mmdata.cm_mnbc_sound_ind, payload_size, plugn.encode_big_endianness ? Endianness::big : Endianness:: machine);
    else if (mmtype == mme::CM_ATTEN_CHAR_IND)
     payload_bytes_written = write(payload.nodes(), mme_msg.mmdata.cm_atten_char_ind, payload_size, plugn.encode_big_endianness ? Endianness::big : Endianness:: machine);
    else if (mmtype == mme::CM_ATTEN_CHAR_RSP)
     payload_bytes_written = write(payload.nodes(), mme_msg.mmdata.cm_atten_char_rsp, payload_size, plugn.encode_big_endianness ? Endianness::big : Endianness:: machine);
    else if (mmtype == mme::CM_ATTEN_PROFILE_IND)
     payload_bytes_written = write(payload.nodes(), mme_msg.mmdata.cm_atten_profile_ind, payload_size, plugn.encode_big_endianness ? Endianness::big : Endianness:: machine);
    else if (mmtype == mme::CM_VALIDATE_REQ)
     payload_bytes_written = write(payload.nodes(), mme_msg.mmdata.cm_validate_req, payload_size, plugn.encode_big_endianness ? Endianness::big : Endianness:: machine);
    else if (mmtype == mme::CM_VALIDATE_CNF)
     payload_bytes_written = write(payload.nodes(), mme_msg.mmdata.cm_validate_cnf, payload_size, plugn.encode_big_endianness ? Endianness::big : Endianness:: machine);
    else if (mmtype == mme::CM_SLAC_MATCH_REQ)
     payload_bytes_written = write(payload.nodes(), mme_msg.mmdata.cm_slac_match_req, payload_size, plugn.encode_big_endianness ? Endianness::big : Endianness:: machine);
    else if (mmtype == mme::CM_SLAC_MATCH_CNF)
     payload_bytes_written = write(payload.nodes(), mme_msg.mmdata.cm_slac_match_cnf, payload_size, plugn.encode_big_endianness ? Endianness::big : Endianness:: machine);
    else if (mmtype == mme::CM_SET_KEY_REQ)
     payload_bytes_written = write(payload.nodes(), mme_msg.mmdata.cm_set_key_req, payload_size, plugn.encode_big_endianness ? Endianness::big : Endianness:: machine);
    else if (mmtype == mme::CM_AMP_MAP_REQ)
     payload_bytes_written = write(payload.nodes(), mme_msg.mmdata.cm_amp_map_req, payload_size, plugn.encode_big_endianness ? Endianness::big : Endianness:: machine);
    else if (mmtype == mme::CM_AMP_MAP_CNF)
     payload_bytes_written = write(payload.nodes(), mme_msg.mmdata.cm_amp_map_cnf, payload_size, plugn.encode_big_endianness ? Endianness::big : Endianness:: machine);
    
    if (print_debug) 
      std::cout << mme_msg << std::endl;

    int commfd = -1;

    sockaddr_in last_client = {0};
    socklen_t last_client_len{};
    {
      std::lock_guard g{plugn.commfd_mtx};
      commfd = plugn.commfd;
      last_client = plugn.last_client;
      last_client_len = plugn.last_client_len;
    }

    if (commfd < 0) {
      return nullptr;
    }
    uint32_t flags{0};
    if (plugn.encode_vlan_tag){
      auto r = sctp_sendmsg(commfd, 
                            mme_msg_buffer, 
                            std::max((size_t)60, 
                            (size_t)payload_bytes_written + sizeof(homeplug_mme_generic_header)),
                            (sockaddr*) &last_client,
                            last_client_len, 0, flags, 0, 0, 0);
    } else {
      memmove(mme_msg_buffer + sizeof(homeplug_mme_generic::vlan_tag), mme_msg_buffer, sizeof(homeplug_mme_header_mac_section));
      auto r = sctp_sendmsg(commfd, 
                            mme_msg_buffer + sizeof(homeplug_mme_generic::vlan_tag), 
                            std::max((size_t)60, 
                            (size_t)payload_bytes_written + sizeof(homeplug_mme_generic_header) - sizeof(homeplug_mme_generic::vlan_tag)),
                            (sockaddr*) &last_client,
                            last_client_len, 0, flags, 0, 0, 0);
    }
    return nullptr;
}

void mme4ceps_plugin::send_message(ceps::ast::Struct_ptr ceps_msg){
    using namespace ceps::ast;
    auto msg_name = ceps::ast::name(*ceps_msg);
    bool found{};
    bool no_connection{};
    bool print_debug {};


    int commfd = -1;
    sockaddr_in client = {0};
    socklen_t client_len{};
    Channel ch{};
    {
      std::lock_guard g{commfd_mtx};
      commfd = plugn.commfd;
      client = plugn.last_client;
      client_len = plugn.last_client_len;
      no_connection = commfd == -1;
      if (no_connection) 
        std::cerr << "*** Error [V2G MME Plugin: send_v2g_low_level(" << msg_name<<"): No remote host." <<"\n";
      
      if (!no_connection){
        auto it = channels.begin();
        for(;it != channels.end();++it){
          if (it->second.message.name == msg_name) { ch = it->second;break;}
        }
        found = it != channels.end();
      }
    }
   
    if (no_connection)
    {
      std::cerr << "*** Error [V2G MME Plugin: send_v2g_low_level("<< msg_name << "{...}) call]: No connection.\n";
      return; 
    }
    else if (!found)
    {
      std::cerr << "*** Error [V2G MME Plugin: send_v2g_low_level("<< msg_name << "{...}) call]: No channel accepts this message.\n";
      return; 
    }

    std::vector<char> data;
    std::vector< std::pair< size_t , size_t> > positions {};
    std::pair< size_t , size_t> last_pair {};

    for(auto p: children(*ceps_msg)){
      if (!is<Ast_node_kind::structdef>(p)) continue;
      auto field_name = ceps::ast::name(as_struct_ref(p));
      short field_ctr = 1;
      bool field_name_matched = false;

      for(auto const & field: ch.message.fields){
        if (field.name != field_name) {
          ++field_ctr; 
          continue; 
        }
        field_name_matched = true;

        auto enc_size = sizeof(short) + (field.type == Channel::type_encoding::i32 ? 
                                            sizeof(std::int32_t) : (field.type == Channel::type_encoding::d64 ? sizeof(std::double_t) : 4 ) );

        if (print_debug) std::cout << " last_pair is (" << last_pair.first << "/" << last_pair.second << ")\n";
        if (print_debug) std::cout << " enc_size is (" << enc_size << ")\n";        

        last_pair = {last_pair.first+last_pair.second, enc_size };

        if (print_debug) std::cout << " data size is " << data.size() << "\n";

        data.resize(data.size() + enc_size);

        if (print_debug) std::cout << " last_pair is (" << last_pair.first << "/" << last_pair.second << ")\n";
        if (print_debug) std::cout << " data size is " << data.size() << "\n";

        if (field.type == Channel::type_encoding::i32) {
          *((short*) (data.data() + last_pair.first)) = field_ctr;
          std::int32_t val{};
          if (children(as_struct_ref(p)).size())
           if ( is<Ast_node_kind::int_literal>(children(as_struct_ref(p))[0]))
            val = value(as_int_ref(children(as_struct_ref(p))[0]));
           else 
            std::cerr << "*** Warning [V2G MME Plugin: send_v2g_low_level("<< msg_name << "{...}) call]: Type mismatch (" << field.name << ").\n";
          *((std::int32_t*)(data.data() + last_pair.first + sizeof(short)) ) = val;
          
          if (print_debug) std::cout << " value is " << val << "\n";        

        } else if (field.type == Channel::type_encoding::d64) {
          *((short*)(data.data() + last_pair.first)) = field_ctr;
          std::double_t val{};
          if (children(as_struct_ref(p)).size())
           if ( is<Ast_node_kind::float_literal>(children(as_struct_ref(p))[0]))
            val = value(as_double_ref(children(as_struct_ref(p))[0]));
           else
            std::cerr << "*** Warning [V2G MME Plugin: send_v2g_low_level("<< msg_name << "{...}) call]: Type mismatch (" << field.name << ").\n";
          *((std::double_t*)(data.data() + last_pair.first + sizeof(short)) ) = val;
          if (print_debug) std::cout << " value is " << val << "\n";        
        }
        positions.push_back(last_pair);        
      }
      
      if (!field_name_matched){
        std::cerr << "*** Warning [V2G MME Plugin: send_v2g_low_level("<< msg_name << "{...}) call]: Unknown field defined (" << field_name << ").\n";
      }
    }
    if (!positions.size()) return;    
    sctp_sndrcvinfo sri = {0};
    sri.sinfo_stream = ch.chidx;
    int msg_idx = 1;
    int msg_flags{};
    if (print_debug) for(size_t i = 0; i < data.size(); ++i) 
      std::cerr <<"data["<<i<<"] = " << (int) data[i] << "\n";

    auto r = sctp_sendmsg(commfd, &msg_idx,sizeof(msg_idx),(sockaddr*) &client,client_len,0,0,sri.sinfo_stream,0,0 ) ;
    if (r < 0){
      std::cerr << "*** Error [V2G MME Plugin: send_v2g_low_level("<< msg_name << "{...}) call]: Failed to send message.\n";
    }
    int eof = -1;
    for (auto p : positions){
      r = sctp_sendmsg(commfd, data.data() + p.first ,p.second,(sockaddr*) &client,client_len,0,0,sri.sinfo_stream,0,0 ) ;
      if (r < 0){
        std::cerr << "*** Error [V2G MME Plugin: send_v2g_low_level("<< msg_name << "{...}) call]: Failed to send message.\n";
      }   
    }
    r = sctp_sendmsg(commfd, &eof,sizeof(eof),(sockaddr*) &client,client_len,0,0,sri.sinfo_stream,0,0 ) ;
    if (r < 0){
      std::cerr << "*** Error [V2G MME Plugin: send_v2g_low_level("<< msg_name << "{...}) call]: Failed to send message.\n";
    }
}

static ceps::ast::node_t plugin_send_generic(ceps::ast::node_callparameters_t params){
    auto msg = get_first_child(params);
    if (!ceps::ast::is<ceps::ast::Ast_node_kind::structdef>(msg)) return nullptr;
    auto ceps_msg = ceps::ast::as_struct_ptr(msg);
    plugn.send_message(ceps_msg);
    return nullptr;
}

extern "C" void init_plugin(IUserdefined_function_registry* smc)
{
  plugn.ceps_engine = plugn.plugin_master = plugin_master = smc->get_plugin_interface(); 
  plugin_master->reg_ceps_plugin("homeplug_mme_handler", plugin_entrypoint_route_mme);
  plugin_master->reg_ceps_plugin("send_homeplug_mme",plugin_send_mme);
  plugin_master->reg_ceps_plugin("send_v2g_low_level",plugin_send_generic);
  plugin_master->run_indefinitely(true);
}








