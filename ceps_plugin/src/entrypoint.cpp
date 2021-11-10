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

#include "mme4ceps.hpp"
#include "mme2ceps_utils.hpp"
#include "ceps2mme.hpp"
#include "tests/test_handle_homeplug_mme.hpp"

static Ism4ceps_plugin_interface* plugin_master = nullptr;
static mme4ceps_plugin plugn;

static ceps::ast::node_t plugin_entrypoint_route_mme(ceps::ast::node_callparameters_t params){
    auto eval_ceps_handler = [&](std::vector<ceps::ast::node_t> v){
      for(auto e: v){
        ceps::ast::function_target_t func_id; 
        ceps::ast::nodes_t args;
        if (is_a_funccall(e,func_id,args)){
            plugn.ceps_engine->queue_internal_event(func_id,args);
        } else if (ceps::ast::is_a_symbol(e)){
          auto& sym = ceps::ast::as_symbol_ref(e);
          if (ceps::ast::kind(sym) == "Event") plugn.ceps_engine->queue_event(ceps::ast::name(sym),{}); 
        }
      }
    };
    auto t = get_first_child(params);
    if (t) plugn.set_associated_ceps_block(t->clone());
    auto on_initplugin =  ceps::ast::Nodeset{t}["setup"]["on_initplugin"].nodes();
    eval_ceps_handler(on_initplugin);
  
    if (ceps::ast::Nodeset{t}["setup"]["run_tests"].nodes().size()){
      auto on_start_tests =  ceps::ast::Nodeset{t}["setup"]["on_start_tests"].nodes();
      auto on_end_tests =  ceps::ast::Nodeset{t}["setup"]["on_end_tests"].nodes();
      eval_ceps_handler(on_start_tests);
      tests::handle_homeplug_mme::run_all(plugn);
      eval_ceps_handler(on_end_tests);
    }
    return nullptr;
}

static ceps::ast::node_t plugin_send_mme(ceps::ast::node_callparameters_t params){
    auto msg = get_first_child(params);
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
    auto print_debug = ns["debug"].size() > 0;
        
    if (!mme_type.has_value()) return nullptr;
    char mme_msg_buffer[sizeof(homeplug_mme_generic)*2] = {0};
    homeplug_mme_generic& mme_msg = *((homeplug_mme_generic*)mme_msg_buffer);
    auto payload_size = sizeof(homeplug_mme_generic) - sizeof(homeplug_mme_generic_header);

    mme_msg.mmtype = mme_type.value();
    if(fmi.has_value()) mme_msg.fmi = fmi.value();
    if(fmsn.has_value()) mme_msg.fmsn = fmsn.value();
    if(mmv.has_value()) mme_msg.mmv = mmv.value();
    if(mtype.has_value()) mme_msg.mtype = mtype.value();

    if(!osa.empty()) write_bytes(osa.nodes(), ((uint8_t*) &mme_msg.osa), ((uint8_t*) &mme_msg.osa) + sizeof(mme_msg.osa));
    if(!oda.empty()) write_bytes(oda.nodes(), ((uint8_t*) &mme_msg.oda), ((uint8_t*) &mme_msg.oda) + sizeof(mme_msg.oda));
    if(vlan_tag.has_value()) mme_msg.vlan_tag = vlan_tag.value();

    if (mme_msg.mmtype == mme::CM_SLAC_PARM_REQ)
     write(payload.nodes(), mme_msg.mmdata.cm_slac_parm_req, payload_size);
    else if (mme_msg.mmtype == mme::CM_SLAC_PARM_CNF)
     write(payload.nodes(), mme_msg.mmdata.cm_slac_parm_cnf, payload_size);
    else if (mme_msg.mmtype == mme::CM_START_ATTEN_CHAR_IND)
     write(payload.nodes(), mme_msg.mmdata.cm_start_atten_char_ind, payload_size);
    else if (mme_msg.mmtype == mme::CM_MNBC_SOUND_IND)
     write(payload.nodes(), mme_msg.mmdata.cm_mnbc_sound_ind, payload_size);
    else if (mme_msg.mmtype == mme::CM_ATTEN_CHAR_IND)
     write(payload.nodes(), mme_msg.mmdata.cm_atten_char_ind, payload_size);
    else if (mme_msg.mmtype == mme::CM_ATTEN_CHAR_RSP)
     write(payload.nodes(), mme_msg.mmdata.cm_atten_char_rsp, payload_size);
    else if (mme_msg.mmtype == mme::CM_ATTEN_PROFILE_IND)
     write(payload.nodes(), mme_msg.mmdata.cm_atten_profile_ind, payload_size);
    else if (mme_msg.mmtype == mme::CM_VALIDATE_REQ)
     write(payload.nodes(), mme_msg.mmdata.cm_validate_req, payload_size);
    else if (mme_msg.mmtype == mme::CM_VALIDATE_CNF)
     write(payload.nodes(), mme_msg.mmdata.cm_validate_cnf, payload_size);
    else if (mme_msg.mmtype == mme::CM_SLAC_MATCH_REQ)
     write(payload.nodes(), mme_msg.mmdata.cm_slac_match_req, payload_size);
    else if (mme_msg.mmtype == mme::CM_SLAC_MATCH_CNF)
     write(payload.nodes(), mme_msg.mmdata.cm_slac_match_cnf, payload_size);
    else if (mme_msg.mmtype == mme::CM_SET_KEY_REQ)
     write(payload.nodes(), mme_msg.mmdata.cm_set_key_req, payload_size);
    else if (mme_msg.mmtype == mme::CM_AMP_MAP_REQ)
     write(payload.nodes(), mme_msg.mmdata.cm_amp_map_req, payload_size);
    else if (mme_msg.mmtype == mme::CM_AMP_MAP_CNF)
     write(payload.nodes(), mme_msg.mmdata.cm_amp_map_cnf, payload_size);
    
    if (print_debug) 
      std::cout << mme_msg << std::endl;

    return nullptr;
}

extern "C" void init_plugin(IUserdefined_function_registry* smc)
{
  
  plugn.ceps_engine = plugn.plugin_master = plugin_master = smc->get_plugin_interface(); 
  plugin_master->reg_ceps_plugin("homeplug_mme_handler", plugin_entrypoint_route_mme);
  plugin_master->reg_ceps_plugin("send_homeplug_mme",plugin_send_mme);
}
