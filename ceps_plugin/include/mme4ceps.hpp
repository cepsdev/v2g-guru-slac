/*
MIT License

Copyright (c) 2021,2022 Tomas Prerovsky

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

#pragma once

#include <stdlib.h>
#include <array>
#include <ctype.h>
#include "ceps_ast.hh" 
#include "core/include/state_machine_simulation_core_reg_fun.hpp"
#include "core/include/state_machine_simulation_core_plugin_interface.hpp"
#include "core/include/state_machine_simulation_core.hpp"
#include "mme.hpp"
#include <functional>

namespace std{

template<>
 struct hash<sockaddr>{
   hash<unsigned long long> h;
   size_t operator() (const sockaddr& v) const {
     return h( *((long long *) &v))  ;
   }
 };
}


class mme4ceps_plugin{
    void init();
    std::map<uint16_t, bool (mme4ceps_plugin::*) (homeplug_mme_generic*, size_t mme_size) > mme_msg_to_symbol_table_setup_routines;
  public:
    struct result_t{
      bool ok;
      std::string msg;
    };

    /*
    channel_info_t represents a logical connection between us and a remote site
    */

    using channel_t = short;

    struct logical_connection_info_t{
        enum type: short{
            i32 = 5,
            i64,
            d64,
            str
        };
        logical_connection_info_t() = default;
        logical_connection_info_t(sockaddr client,socklen_t len):client{client},len{len}{}
        bool valid = false;
        sockaddr client;
        socklen_t len;
        channel_t channel = 0;
        bool generic = false;
        std::string msg_name;
        std::vector<std::string> fields;
        std::vector<short> field_types;
    };

    std::unordered_map<sockaddr, std::vector<logical_connection_info_t>> logical_connections;
    std::mutex logical_connections_mtx;

    void register_client(logical_connection_info_t conn);

    ceps::parser_env::Scope scope;    
    ceps::ast::node_t associated_ceps_block = nullptr;
    ceps::ast::Nodeset on_client_connect; 

    Ism4ceps_plugin_interface* ceps_engine = nullptr;
    Ism4ceps_plugin_interface* plugin_master = nullptr;
    int commfd = -1;
    sockaddr_in last_client = {0};
    socklen_t last_client_len = 0;
    std::mutex commfd_mtx;
    bool encode_vlan_tag = true;
    bool encode_big_endianness = false;

    bool mme_msg_cm_slac_param_req_setup_symtbl(homeplug_mme_generic*, size_t mme_size);
    bool mme_msg_cm_slac_parm_cnf_setup_symtbl(homeplug_mme_generic*, size_t mme_size);
    bool mme_msg_cm_start_atten_char_ind_setup_symtbl(homeplug_mme_generic*, size_t mme_size);
    bool mme_msg_cm_mnbc_sound_ind_setup_symtbl(homeplug_mme_generic*, size_t mme_size);
    bool mme_msg_cm_atten_char_ind_setup_symtbl(homeplug_mme_generic*, size_t mme_size);
    bool mme_msg_cm_atten_profile_ind_setup_symtbl(homeplug_mme_generic*, size_t mme_size);
    bool mme_msg_cm_atten_char_rsp_setup_symtbl(homeplug_mme_generic*, size_t mme_size);
    bool mme_msg_cm_validate_req_setup_symtbl(homeplug_mme_generic*, size_t mme_size);
    bool mme_msg_cm_validate_cnf_setup_symtbl(homeplug_mme_generic*, size_t mme_size);
    bool mme_msg_cm_slac_match_req_setup_symtbl(homeplug_mme_generic*, size_t mme_size);
    bool mme_msg_cm_slac_match_cnf_setup_symtbl(homeplug_mme_generic*, size_t mme_size);
    bool mme_msg_cm_set_key_req_setup_symtbl(homeplug_mme_generic*, size_t mme_size);
    bool mme_msg_cm_set_key_cnf_setup_symtbl(homeplug_mme_generic*, size_t mme_size);
    bool mme_msg_cm_amp_map_req_setup_symtbl(homeplug_mme_generic*, size_t mme_size);
    bool mme_msg_cm_amp_map_cnf_setup_symtbl(homeplug_mme_generic*, size_t mme_size);
    /**
    * The plugin is associated with a function name in the ceps context, each time this function gets called the plugin will be reinitialized using
    * the single argument of the call which is called associated ceps block. 
     ***/
    void set_associated_ceps_block(ceps::ast::node_t v);
    void handle_homeplug_mme(homeplug_mme_generic*, size_t mme_size);

    void execute_ceps(ceps::ast::Nodeset);

    result_t start_sctp_server(std::string port);
};
