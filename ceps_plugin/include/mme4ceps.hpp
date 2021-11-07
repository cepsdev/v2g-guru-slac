#include <stdlib.h>
#include <ctype.h>
#include "ceps_ast.hh" 
#include "core/include/state_machine_simulation_core_reg_fun.hpp"
#include "core/include/state_machine_simulation_core_plugin_interface.hpp"
#include "core/include/state_machine_simulation_core.hpp"
#include "mme.hpp"

class mme4ceps_plugin{
    void init();
    std::map<uint16_t, bool (mme4ceps_plugin::*) (homeplug_mme_generic*, size_t mme_size) > mme_msg_to_symbol_table_setup_routines;
  public:
    ceps::parser_env::Scope scope;    
    ceps::ast::node_t associated_ceps_block = nullptr;
    Ism4ceps_plugin_interface* ceps_engine = nullptr;
    Ism4ceps_plugin_interface* plugin_master = nullptr;

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
};
