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

#include "ceps2mme.hpp"

/*
uint16 

*/

size_t write_uint16(ceps::ast::Struct_ptr strct,std::uint16_t* pv, Endianness endianness) 
{
  using namespace ceps::ast;
  auto wr = [&](node_t nd,std::uint16_t* p){
    if (is<ceps::ast::Ast_node_kind::int_literal>(nd)){
      *p = value(as_int_ref(nd));
    } else if (is<ceps::ast::Ast_node_kind::long_literal>(nd)){
      *p = value(as_int64_ref(nd));
    } else if (is<ceps::ast::Ast_node_kind::float_literal>(nd)){
      *p = value(as_double_ref(nd));
    } else if (is<ceps::ast::Ast_node_kind::string_literal>(nd)){
      auto const & v = value(as_string_ref(nd));
      if (v.length())
       * ((char*)pv) = v[0];
      if (v.length() > 1)
       * ( ((char*)pv) + 1) = v[1];
    }
    return sizeof(std::uint16_t);
  };

  size_t written = 0;
  if (!num_of_children(*strct)) return written;
  if ( 1 == num_of_children(*strct)) {
    written = wr(children(*strct)[0], pv);
    if (endianness == Endianness::big) *pv = htobe16(*pv);
  } else {
    std::uint16_t t1{},t2{};
    written = wr(children(*strct)[0], &t1);
    wr(children(*strct)[1], &t2 );
    *pv = (t1 & 0xFF) | (t2 << 8);
  }
  return written;
}


size_t write(std::vector<ceps::ast::Nodebase_ptr> const & v, cm_slac_parm_req_t& msg, size_t size, Endianness endianness){
  size_t written = 0;
  for(auto e : v){
    if (!ceps::ast::is<ceps::ast::Ast_node_kind::structdef>(e)) continue;
    auto& strct = *ceps::ast::as_struct_ptr(e);
    auto& name = ceps::ast::name(strct);

    if (name == "application_type") 
     written += write_bytes(&strct,((uint8_t*)&msg.application_type),((uint8_t*)&msg.application_type) + sizeof(msg.application_type));
    else if (name == "run_id")
     written += write_bytes(&strct,((uint8_t*)&msg.run_id),((uint8_t*)&msg.run_id) + sizeof(msg.run_id));
    else if (name == "security_type")
     written += write_bytes(&strct,((uint8_t*)&msg.security_type),((uint8_t*)&msg.security_type) + sizeof(msg.security_type));    
  }
  return written;
} 

size_t write(std::vector<ceps::ast::Nodebase_ptr> const & v, cm_slac_parm_cnf_t& msg, size_t size, Endianness endianness){
  size_t written = 0;
  for(auto e : v){
    if (!ceps::ast::is<ceps::ast::Ast_node_kind::structdef>(e)) continue;
    auto& strct = *ceps::ast::as_struct_ptr(e);
    auto& name = ceps::ast::name(strct);

    if (name == "application_type") 
     written += write_bytes(&strct,((uint8_t*)&msg.application_type),((uint8_t*)&msg.application_type) + sizeof(msg.application_type));
    else if (name == "run_id")
     written += write_bytes(&strct,((uint8_t*)&msg.run_id),((uint8_t*)&msg.run_id) + sizeof(msg.run_id));
    else if (name == "security_type")
     written += write_bytes(&strct,((uint8_t*)&msg.security_type),((uint8_t*)&msg.security_type) + sizeof(msg.security_type));
    else if (name == "num_sounds")
     written += write_bytes(&strct,((uint8_t*)&msg.num_sounds),((uint8_t*)&msg.num_sounds) + sizeof(msg.num_sounds));
    else if (name == "time_out")
     written += write_bytes(&strct,((uint8_t*)&msg.time_out),((uint8_t*)&msg.time_out) + sizeof(msg.time_out));
    else if (name == "m_sound_target")
     written += write_bytes(&strct,((uint8_t*)&msg.m_sound_target),((uint8_t*)&msg.m_sound_target) + sizeof(msg.m_sound_target));
    else if (name == "forwarding_sta")
     written += write_bytes(&strct,((uint8_t*)&msg.forwarding_sta),((uint8_t*)&msg.forwarding_sta) + sizeof(msg.forwarding_sta));
    else if (name == "resp_type")
     written += write_bytes(&strct,((uint8_t*)&msg.resp_type),((uint8_t*)&msg.resp_type) + sizeof(msg.resp_type));
  }
  return written;
} 

size_t write(std::vector<ceps::ast::Nodebase_ptr> const & v, cm_start_atten_char_ind_t& msg, size_t size, Endianness endianness){
  size_t written = 0;
  for(auto e : v){
    if (!ceps::ast::is<ceps::ast::Ast_node_kind::structdef>(e)) continue;
    auto& strct = *ceps::ast::as_struct_ptr(e);
    auto& name = ceps::ast::name(strct);

    if (name == "application_type") 
     written += write_bytes(&strct,((uint8_t*)&msg.application_type),((uint8_t*)&msg.application_type) + sizeof(msg.application_type));
    else if (name == "run_id")
     written += write_bytes(&strct,((uint8_t*)&msg.run_id),((uint8_t*)&msg.run_id) + sizeof(msg.run_id));
    else if (name == "security_type")
     written += write_bytes(&strct,((uint8_t*)&msg.security_type),((uint8_t*)&msg.security_type) + sizeof(msg.security_type));    
    else if (name == "forwarding_sta")
     written += write_bytes(&strct,((uint8_t*)&msg.forwarding_sta),((uint8_t*)&msg.forwarding_sta) + sizeof(msg.forwarding_sta));    
    else if (name == "resp_type")
     written += write_bytes(&strct,((uint8_t*)&msg.resp_type),((uint8_t*)&msg.resp_type) + sizeof(msg.resp_type));    
    else if (name == "time_out")
     written += write_bytes(&strct,((uint8_t*)&msg.time_out),((uint8_t*)&msg.time_out) + sizeof(msg.time_out));    
    else if (name == "num_sounds")
     written += write_bytes(&strct,((uint8_t*)&msg.num_sounds),((uint8_t*)&msg.num_sounds) + sizeof(msg.num_sounds));   

  }
  return written;
} 


size_t write(std::vector<ceps::ast::Nodebase_ptr> const & v, cm_atten_char_ind_t& msg, size_t size, Endianness endianness){
  size_t written = 0;
  for(auto e : v){
    if (!ceps::ast::is<ceps::ast::Ast_node_kind::structdef>(e)) continue;
    auto& strct = *ceps::ast::as_struct_ptr(e);
    auto& name = ceps::ast::name(strct);

    if (name == "application_type") 
     written += write_bytes(&strct,((uint8_t*)&msg.application_type),((uint8_t*)&msg.application_type) + sizeof(msg.application_type));
    else if (name == "run_id")
     written += write_bytes(&strct,((uint8_t*)&msg.run_id),((uint8_t*)&msg.run_id) + sizeof(msg.run_id));
    else if (name == "security_type")
     written += write_bytes(&strct,((uint8_t*)&msg.security_type),((uint8_t*)&msg.security_type) + sizeof(msg.security_type));    
    else if (name == "source_id")
     written += write_bytes(&strct,((uint8_t*)&msg.source_id),((uint8_t*)&msg.source_id) + sizeof(msg.source_id));    
    else if (name == "source_address")
     written += write_bytes(&strct,((uint8_t*)&msg.source_address),((uint8_t*)&msg.source_address) + sizeof(msg.source_address));    
    else if (name == "resp_id")
     written += write_bytes(&strct,((uint8_t*)&msg.resp_id),((uint8_t*)&msg.resp_id) + sizeof(msg.resp_id));    
    else if (name == "num_sounds")
     written += write_bytes(&strct,((uint8_t*)&msg.num_sounds),((uint8_t*)&msg.num_sounds) + sizeof(msg.num_sounds));    
    else if (name == "atten_profile")
     written += write_bytes(&strct,((uint8_t*)&msg.atten_profile),((uint8_t*)&msg.atten_profile) + sizeof(msg.atten_profile));    
  }
  return written;
} 

size_t write(std::vector<ceps::ast::Nodebase_ptr> const & v, cm_mnbc_sound_ind_t& msg, size_t size, Endianness endianness){
  size_t written = 0;
  for(auto e : v){
    if (!ceps::ast::is<ceps::ast::Ast_node_kind::structdef>(e)) continue;
    auto& strct = *ceps::ast::as_struct_ptr(e);
    auto& name = ceps::ast::name(strct);

    if (name == "application_type") 
     written += write_bytes(&strct,((uint8_t*)&msg.application_type),((uint8_t*)&msg.application_type) + sizeof(msg.application_type));
    else if (name == "run_id")
     written += write_bytes(&strct,((uint8_t*)&msg.run_id),((uint8_t*)&msg.run_id) + sizeof(msg.run_id));
    else if (name == "security_type")
     written += write_bytes(&strct,((uint8_t*)&msg.security_type),((uint8_t*)&msg.security_type) + sizeof(msg.security_type));    
    else if (name == "sender_id")
     written += write_bytes(&strct,((uint8_t*)&msg.sender_id),((uint8_t*)&msg.sender_id) + sizeof(msg.sender_id));    
    else if (name == "cnt")
     written += write_bytes(&strct,((uint8_t*)&msg.cnt),((uint8_t*)&msg.cnt) + sizeof(msg.cnt));    
    else if (name == "rnd")
     written += write_bytes(&strct,((uint8_t*)&msg.rnd),((uint8_t*)&msg.rnd) + sizeof(msg.rnd));    

  }
  return written;
} 

size_t write(std::vector<ceps::ast::Nodebase_ptr> const & v, cm_atten_char_rsp_t& msg, size_t size, Endianness endianness){
  size_t written = 0;
  for(auto e : v){
    if (!ceps::ast::is<ceps::ast::Ast_node_kind::structdef>(e)) continue;
    auto& strct = *ceps::ast::as_struct_ptr(e);
    auto& name = ceps::ast::name(strct);

    if (name == "application_type") 
     written += write_bytes(&strct,((uint8_t*)&msg.application_type),((uint8_t*)&msg.application_type) + sizeof(msg.application_type));
    else if (name == "run_id")
     written += write_bytes(&strct,((uint8_t*)&msg.run_id),((uint8_t*)&msg.run_id) + sizeof(msg.run_id));
    else if (name == "security_type")
     written += write_bytes(&strct,((uint8_t*)&msg.security_type),((uint8_t*)&msg.security_type) + sizeof(msg.security_type));    
    else if (name == "source_address")
     written += write_bytes(&strct,((uint8_t*)&msg.source_address),((uint8_t*)&msg.source_address) + sizeof(msg.source_address));    
    else if (name == "source_id")
     written += write_bytes(&strct,((uint8_t*)&msg.source_id),((uint8_t*)&msg.source_id) + sizeof(msg.source_id));    
    else if (name == "resp_id")
     written += write_bytes(&strct,((uint8_t*)&msg.resp_id),((uint8_t*)&msg.resp_id) + sizeof(msg.resp_id));    
    else if (name == "result")
     written += write_bytes(&strct,((uint8_t*)&msg.result),((uint8_t*)&msg.result) + sizeof(msg.result));    
  }
  return written;
} 


size_t write(std::vector<ceps::ast::Nodebase_ptr> const & v, cm_atten_profile_ind_t& msg, size_t size, Endianness endianness){
  size_t written = 0;
  for(auto e : v){
    if (!ceps::ast::is<ceps::ast::Ast_node_kind::structdef>(e)) continue;
    auto& strct = *ceps::ast::as_struct_ptr(e);
    auto& name = ceps::ast::name(strct);

    if (name == "pev_mac") 
     written += write_bytes(&strct,((uint8_t*)&msg.pev_mac),((uint8_t*)&msg.pev_mac) + sizeof(msg.pev_mac));
    else if (name == "num_groups")
     written += write_bytes(&strct,((uint8_t*)&msg.num_groups),((uint8_t*)&msg.num_groups) + sizeof(msg.num_groups));
    else if (name == "aag")
     written += write_bytes(&strct,((uint8_t*)&msg.aag),((uint8_t*)&msg.aag) + sizeof(msg.aag));    
  }
  return written;
} 

size_t write(std::vector<ceps::ast::Nodebase_ptr> const & v, cm_validate_req_t& msg, size_t size, Endianness endianness){
  size_t written = 0;
  for(auto e : v){
    if (!ceps::ast::is<ceps::ast::Ast_node_kind::structdef>(e)) continue;
    auto& strct = *ceps::ast::as_struct_ptr(e);
    auto& name = ceps::ast::name(strct);

    if (name == "signal_type") 
     written += write_bytes(&strct,((uint8_t*)&msg.signal_type),((uint8_t*)&msg.signal_type) + sizeof(msg.signal_type));
    else if (name == "timer")
     written += write_bytes(&strct,((uint8_t*)&msg.timer),((uint8_t*)&msg.timer) + sizeof(msg.timer));
    else if (name == "result")
     written += write_bytes(&strct,((uint8_t*)&msg.result),((uint8_t*)&msg.result) + sizeof(msg.result));    
  }
  return written;
} 


size_t write(std::vector<ceps::ast::Nodebase_ptr> const & v, cm_validate_cnf_t& msg, size_t size, Endianness endianness){
  size_t written = 0;
  for(auto e : v){
    if (!ceps::ast::is<ceps::ast::Ast_node_kind::structdef>(e)) continue;
    auto& strct = *ceps::ast::as_struct_ptr(e);
    auto& name = ceps::ast::name(strct);

    if (name == "signal_type") 
     written += write_bytes(&strct,((uint8_t*)&msg.signal_type),((uint8_t*)&msg.signal_type) + sizeof(msg.signal_type));
    else if (name == "toggle_num")
     written += write_bytes(&strct,((uint8_t*)&msg.toggle_num),((uint8_t*)&msg.toggle_num) + sizeof(msg.toggle_num));
    else if (name == "result")
     written += write_bytes(&strct,((uint8_t*)&msg.result),((uint8_t*)&msg.result) + sizeof(msg.result));    
  }
  return written;
} 


size_t write(std::vector<ceps::ast::Nodebase_ptr> const & v, cm_slac_match_req_t& msg, size_t size, Endianness endianness){
  size_t written = 0;
  for(auto e : v){
    if (!ceps::ast::is<ceps::ast::Ast_node_kind::structdef>(e)) continue;
    auto& strct = *ceps::ast::as_struct_ptr(e);
    auto& name = ceps::ast::name(strct);

    if (name == "application_type") 
     written += write_bytes(&strct,((uint8_t*)&msg.application_type),((uint8_t*)&msg.application_type) + sizeof(msg.application_type));
    else if (name == "security_type")
     written += write_bytes(&strct,((uint8_t*)&msg.security_type),((uint8_t*)&msg.security_type) + sizeof(msg.security_type));
    else if (name == "mvflength"){
      //written += write_bytes(&strct,((uint8_t*)&msg.mvflength),((uint8_t*)&msg.mvflength) + sizeof(msg.mvflength));
      std::uint16_t t;
      written += write_uint16(&strct,&t,endianness);
      msg.mvflength = t;  
    }
    else if (name == "pev_id") 
     written += write_bytes(&strct,((uint8_t*)&msg.pev_id),((uint8_t*)&msg.pev_id) + sizeof(msg.pev_id));
    else if (name == "pev_mac")
     written += write_bytes(&strct,((uint8_t*)&msg.pev_mac),((uint8_t*)&msg.pev_mac) + sizeof(msg.pev_mac));
    else if (name == "evse_id")
     written += write_bytes(&strct,((uint8_t*)&msg.evse_id),((uint8_t*)&msg.evse_id) + sizeof(msg.evse_id));    
    else if (name == "evse_mac") 
     written += write_bytes(&strct,((uint8_t*)&msg.evse_mac),((uint8_t*)&msg.evse_mac) + sizeof(msg.evse_mac));
    else if (name == "run_id")
     written += write_bytes(&strct,((uint8_t*)&msg.run_id),((uint8_t*)&msg.run_id) + sizeof(msg.run_id));
    else if (name == "rsvd")
     written += write_bytes(&strct,((uint8_t*)&msg.rsvd),((u_int8_t*)&msg.rsvd) + sizeof(msg.rsvd));
  }
  return written;
} 


size_t write(std::vector<ceps::ast::Nodebase_ptr> const & v, cm_slac_match_cnf_t& msg, size_t size, Endianness endianness){
  size_t written = 0;
  for(auto e : v){
    if (!ceps::ast::is<ceps::ast::Ast_node_kind::structdef>(e)) continue;
    auto& strct = *ceps::ast::as_struct_ptr(e);
    auto& name = ceps::ast::name(strct);

    if (name == "application_type") 
     written += write_bytes(&strct,((uint8_t*)&msg.application_type),((uint8_t*)&msg.application_type) + sizeof(msg.application_type));
    else if (name == "security_type")
     written += write_bytes(&strct,((uint8_t*)&msg.security_type),((uint8_t*)&msg.security_type) + sizeof(msg.security_type));
    else if (name == "mvflength"){
     //written += write_bytes(&strct,((uint8_t*)&msg.mvflength),((uint8_t*)&msg.mvflength) + sizeof(msg.mvflength));
     std::uint16_t t;
     written += write_uint16(&strct,&t,endianness);
     msg.mvflength = t;
    }    
    else if (name == "pev_id") 
     written += write_bytes(&strct,((uint8_t*)&msg.pev_id),((uint8_t*)&msg.pev_id) + sizeof(msg.pev_id));
    else if (name == "pev_mac")
     written += write_bytes(&strct,((uint8_t*)&msg.pev_mac),((uint8_t*)&msg.pev_mac) + sizeof(msg.pev_mac));
    else if (name == "evse_id")
     written += write_bytes(&strct,((uint8_t*)&msg.evse_id),((uint8_t*)&msg.evse_id) + sizeof(msg.evse_id));    
    else if (name == "evse_mac") 
     written += write_bytes(&strct,((uint8_t*)&msg.evse_mac),((uint8_t*)&msg.evse_mac) + sizeof(msg.evse_mac));
    else if (name == "run_id")
     written += write_bytes(&strct,((uint8_t*)&msg.run_id),((uint8_t*)&msg.run_id) + sizeof(msg.run_id));
    else if (name == "nid") 
     written += write_bytes(&strct,((uint8_t*)&msg.nid),((uint8_t*)&msg.nid) + sizeof(msg.nid));
    else if (name == "nmk")
     written += write_bytes(&strct,((uint8_t*)&msg.nmk),((uint8_t*)&msg.nmk) + sizeof(msg.nmk));
    else if (name == "rsvd")
     written += write_bytes(&strct,((uint8_t*)&msg.rsvd),((uint8_t*)&msg.rsvd) + sizeof(msg.rsvd));
  }
  return written;
} 

size_t write(std::vector<ceps::ast::Nodebase_ptr> const & v, cm_set_key_req_t& msg, size_t, Endianness endianness ){
  size_t written = 0;
  for(auto e : v){
    if (!ceps::ast::is<ceps::ast::Ast_node_kind::structdef>(e)) continue;
    auto& strct = *ceps::ast::as_struct_ptr(e);
    auto& name = ceps::ast::name(strct);

    if (name == "key_type") 
     written += write_bytes(&strct,((uint8_t*)&msg.key_type),((uint8_t*)&msg.key_type) + sizeof(msg.key_type));
    else if (name == "my_nonce")
     written += write_bytes(&strct,((uint8_t*)&msg.my_nonce),((uint8_t*)&msg.my_nonce) + sizeof(msg.my_nonce));
    else if (name == "your_nonce")
     written += write_bytes(&strct,((uint8_t*)&msg.your_nonce),((uint8_t*)&msg.your_nonce) + sizeof(msg.your_nonce));    
    else if (name == "pid") 
     written += write_bytes(&strct,((uint8_t*)&msg.pid),((uint8_t*)&msg.pid) + sizeof(msg.pid));
    else if (name == "prn"){
     //written += write_bytes(&strct,((uint8_t*)&msg.prn),((uint8_t*)&msg.prn) + sizeof(msg.prn));
     std::uint16_t t;
     written += write_uint16(&strct,&t,endianness);
     msg.prn = t;
    } else if (name == "pmn")
     written += write_bytes(&strct,((uint8_t*)&msg.pmn),((uint8_t*)&msg.pmn) + sizeof(msg.pmn));    
    else if (name == "cco_capability") 
     written += write_bytes(&strct,((uint8_t*)&msg.cco_capability),((uint8_t*)&msg.cco_capability) + sizeof(msg.cco_capability));
    else if (name == "new_eks")
     written += write_bytes(&strct,((uint8_t*)&msg.new_eks),((uint8_t*)&msg.new_eks) + sizeof(msg.new_eks));
    else if (name == "nid") 
     written += write_bytes(&strct,((uint8_t*)&msg.nid),((uint8_t*)&msg.nid) + sizeof(msg.nid));
    else if (name == "new_key")
     written += write_bytes(&strct,((uint8_t*)&msg.new_key),((uint8_t*)&msg.new_key) + sizeof(msg.new_key));
  }
  return written;
} 


size_t write(std::vector<ceps::ast::Nodebase_ptr> const & v, cm_amp_map_req_t& msg, size_t size, Endianness endianness){
  size_t written = 0;
  for(auto e : v){
    if (!ceps::ast::is<ceps::ast::Ast_node_kind::structdef>(e)) continue;
    auto& strct = *ceps::ast::as_struct_ptr(e);
    auto& name = ceps::ast::name(strct);

    if (name == "amlen"){ 
     //written += write_bytes(&strct,((uint8_t*)&msg.amlen),((uint8_t*)&msg.amlen) + sizeof(msg.amlen));
     std::uint16_t t;
     written += write_uint16(&strct,&t,endianness);
     msg.amlen = t;
     break;     
    }
  }
  
  for(auto e : v){
    if (!ceps::ast::is<ceps::ast::Ast_node_kind::structdef>(e)) continue;
    auto& strct = *ceps::ast::as_struct_ptr(e);
    auto& name = ceps::ast::name(strct);

    if (name == "amdata"){
     written += write_nibbles(&strct,((uint8_t*)&msg.amdata),((uint8_t*)&msg.amdata) + std::min( size - sizeof(msg.amlen), (size_t) ((msg.amlen + 1)/2)  ));
     break;
    }
  }

  return written;
} 

size_t write(std::vector<ceps::ast::Nodebase_ptr> const & v, cm_amp_map_cnf_t& msg, size_t size, Endianness endianness){
  size_t written = 0;
  for(auto e : v){
    if (!ceps::ast::is<ceps::ast::Ast_node_kind::structdef>(e)) continue;
    auto& strct = *ceps::ast::as_struct_ptr(e);
    auto& name = ceps::ast::name(strct);
    if (name == "restype") 
     written += write_bytes(&strct,((uint8_t*)&msg.restype),((uint8_t*)&msg.restype) + sizeof(msg.restype));
  }
  return written;
} 
