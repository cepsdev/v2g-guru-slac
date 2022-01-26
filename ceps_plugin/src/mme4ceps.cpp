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



#include "mme4ceps.hpp"
#include "ceps_ast.hh"

void mme4ceps_plugin::execute_ceps(ceps::ast::Nodeset ns){
  for(auto e: ns.nodes()){
      std::string sym_name;
      std::string kind_name; 
      ceps::ast::nodes_t args;
      if (is_a_symbol_with_arguments(e,sym_name,kind_name,args)){
          if (kind_name == "Event") ceps_engine->queue_internal_event(sym_name,args);
      } else if (ceps::ast::is_a_symbol(e)){
          auto& sym = ceps::ast::as_symbol_ref(e);
          if (ceps::ast::kind(sym) == "Event") ceps_engine->queue_event(ceps::ast::name(sym),{}); 
      }
  }
}

void mme4ceps_plugin::init(){
  mme_msg_to_symbol_table_setup_routines = 
                  { 
                      {mme::CM_SLAC_PARM_REQ,&mme4ceps_plugin::mme_msg_cm_slac_param_req_setup_symtbl},
                      {mme::CM_SLAC_PARM_CNF,&mme4ceps_plugin::mme_msg_cm_slac_parm_cnf_setup_symtbl},
                      {mme::CM_START_ATTEN_CHAR_IND,&mme4ceps_plugin::mme_msg_cm_start_atten_char_ind_setup_symtbl},
                      {mme::CM_MNBC_SOUND_IND,&mme4ceps_plugin::mme_msg_cm_mnbc_sound_ind_setup_symtbl},
                      {mme::CM_ATTEN_CHAR_IND,&mme4ceps_plugin::mme_msg_cm_atten_char_ind_setup_symtbl},
                      {mme::CM_ATTEN_PROFILE_IND,&mme4ceps_plugin::mme_msg_cm_atten_profile_ind_setup_symtbl},
                      {mme::CM_ATTEN_CHAR_RSP,&mme4ceps_plugin::mme_msg_cm_atten_char_rsp_setup_symtbl},
                      {mme::CM_VALIDATE_REQ,&mme4ceps_plugin::mme_msg_cm_validate_req_setup_symtbl},
                      {mme::CM_VALIDATE_CNF,&mme4ceps_plugin::mme_msg_cm_validate_cnf_setup_symtbl},
                      {mme::CM_SLAC_MATCH_REQ,&mme4ceps_plugin::mme_msg_cm_slac_match_req_setup_symtbl},
                      {mme::CM_SLAC_MATCH_CNF,&mme4ceps_plugin::mme_msg_cm_slac_match_cnf_setup_symtbl},
                      {mme::CM_SET_KEY_REQ,&mme4ceps_plugin::mme_msg_cm_set_key_req_setup_symtbl},
                      {mme::CM_SET_KEY_CNF,&mme4ceps_plugin::mme_msg_cm_set_key_cnf_setup_symtbl},
                      {mme::CM_AMP_MAP_REQ,&mme4ceps_plugin::mme_msg_cm_amp_map_req_setup_symtbl},
                      {mme::CM_AMP_MAP_CNF,&mme4ceps_plugin::mme_msg_cm_amp_map_cnf_setup_symtbl} 
                    };
}

void mme4ceps_plugin::handle_homeplug_mme(homeplug_mme_generic* msg, size_t mme_size){
  if (mme_size < sizeof(homeplug_mme_generic_header)) return; 
  auto it = mme_msg_to_symbol_table_setup_routines.find(msg->mmtype);
  if (it == mme_msg_to_symbol_table_setup_routines.end()) return;
  if (associated_ceps_block == nullptr) return;
  if (!(this->*(it->second))(msg, mme_size)) return;
  auto ceps_fragment = static_cast<ceps::ast::Nodebase_ptr>(plugin_master->evaluate_fragment_in_global_context_no_symbol_expansion(associated_ceps_block,&scope));
  if (!ceps_fragment) return;
  ceps_fragment = ceps_fragment->clone();

  ceps::ast::Nodeset ns{ceps_fragment};
  auto on_message = ns["setup"]["on_message"];

  for(auto e : on_message.nodes()) {
    std::string sym_name;
    std::string kind_name; 
    ceps::ast::nodes_t args;
    if (is_a_symbol_with_arguments(e,sym_name,kind_name,args)){
      if (kind_name == "Event") plugin_master->queue_internal_event(sym_name,args);
    } else if (ceps::ast::is_a_symbol(e)){
      auto& sym = ceps::ast::as_symbol_ref(e);
      if (ceps::ast::kind(sym) == "Event") plugin_master->queue_internal_event(ceps::ast::name(sym),{}); 
    }
  }

  gc(ceps_fragment);
}


bool operator == (sockaddr const & lhs, sockaddr const & rhs ){
  for(size_t i = 0; i < sizeof(sockaddr); ++ i){
    if ( ((char*) &lhs)[i] != ((char*) &rhs)[i]  ) return false;
  }
  return true;
}

void mme4ceps_plugin::register_client(logical_connection_info_t conn){
  std::lock_guard<std::mutex> g{logical_connections_mtx};
  auto it = logical_connections.find(conn.client);
  if (it == logical_connections.end()){

  }
}


void mme4ceps_plugin::set_associated_ceps_block(ceps::ast::node_t v){
    auto t = associated_ceps_block;
    associated_ceps_block = v;
    init();
    gc(t);
}