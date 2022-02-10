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
#include "mme2ceps_utils.hpp"

bool mme4ceps_plugin::mme_msg_cm_slac_param_req_setup_symtbl(homeplug_mme_generic* msg, size_t mme_size){
    cm_slac_parm_req_t& slac_parm_req = msg->mmdata.cm_slac_parm_req;
    ceps::interpreter::set_val("mme_type",mme::CM_SLAC_PARM_REQ,scope);
    ceps::interpreter::set_val("mme_application_type",slac_parm_req.application_type,scope);
    ceps::interpreter::set_val("mme_security_type",slac_parm_req.security_type,scope);
    ceps::interpreter::set_val("mme_run_id",slac_parm_req.run_id,slac_parm_req.run_id+sizeof(slac_parm_req.run_id),scope);
    return true;
}

bool mme4ceps_plugin::mme_msg_cm_slac_parm_cnf_setup_symtbl(homeplug_mme_generic* msg, size_t mme_size){
    cm_slac_parm_cnf_t& slac_parm_cnf = msg->mmdata.cm_slac_parm_cnf;
    ceps::interpreter::set_val("mme_type",mme::CM_SLAC_PARM_CNF,scope);
    ceps::interpreter::set_val("mme_application_type",slac_parm_cnf.application_type,scope);
    ceps::interpreter::set_val("mme_security_type",slac_parm_cnf.security_type,scope);
    ceps::interpreter::set_val("mme_run_id",slac_parm_cnf.run_id,slac_parm_cnf.run_id+sizeof(slac_parm_cnf.run_id),scope);  
    ceps::interpreter::set_val("mme_m_sound_target",slac_parm_cnf.m_sound_target,slac_parm_cnf.m_sound_target+sizeof(slac_parm_cnf.m_sound_target),scope);  
    ceps::interpreter::set_val("mme_num_sounds",slac_parm_cnf.num_sounds,scope);
    ceps::interpreter::set_val("mme_time_out",slac_parm_cnf.time_out,scope);
    ceps::interpreter::set_val("mme_resp_type",slac_parm_cnf.resp_type,scope);
    ceps::interpreter::set_val("mme_forwarding_sta",slac_parm_cnf.forwarding_sta,slac_parm_cnf.forwarding_sta+sizeof(slac_parm_cnf.forwarding_sta),scope);  
    return true;
}

bool mme4ceps_plugin::mme_msg_cm_start_atten_char_ind_setup_symtbl(homeplug_mme_generic* msg, size_t mme_size){
    cm_start_atten_char_ind_t& cm_start_atten_char_ind = msg->mmdata.cm_start_atten_char_ind;
    ceps::interpreter::set_val("mme_type",mme::CM_START_ATTEN_CHAR_IND,scope);
    ceps::interpreter::set_val("mme_application_type",cm_start_atten_char_ind.application_type,scope);
    ceps::interpreter::set_val("mme_security_type",cm_start_atten_char_ind.security_type,scope);
    ceps::interpreter::set_val("mme_run_id",cm_start_atten_char_ind.run_id,cm_start_atten_char_ind.run_id+sizeof(cm_start_atten_char_ind.run_id),scope);  
    ceps::interpreter::set_val("mme_num_sounds",cm_start_atten_char_ind.num_sounds,scope);  
    ceps::interpreter::set_val("mme_time_out",cm_start_atten_char_ind.time_out,scope);
    ceps::interpreter::set_val("mme_resp_type",cm_start_atten_char_ind.resp_type,scope);
    ceps::interpreter::set_val("mme_forwarding_sta",cm_start_atten_char_ind.forwarding_sta,cm_start_atten_char_ind.forwarding_sta+sizeof(cm_start_atten_char_ind.forwarding_sta),scope);  
    return true;
}

bool mme4ceps_plugin::mme_msg_cm_mnbc_sound_ind_setup_symtbl(homeplug_mme_generic* msg, size_t mme_size){
    cm_mnbc_sound_ind_t& cm_mnbc_sound_ind = msg->mmdata.cm_mnbc_sound_ind;
    ceps::interpreter::set_val("mme_type",mme::CM_MNBC_SOUND_IND,scope);
    ceps::interpreter::set_val("mme_application_type",cm_mnbc_sound_ind.application_type,scope);
    ceps::interpreter::set_val("mme_security_type",cm_mnbc_sound_ind.security_type,scope);
    ceps::interpreter::set_val("mme_run_id",cm_mnbc_sound_ind.run_id,cm_mnbc_sound_ind.run_id+sizeof(cm_mnbc_sound_ind.run_id),scope);  
    ceps::interpreter::set_val("mme_sender_id",cm_mnbc_sound_ind.sender_id,cm_mnbc_sound_ind.sender_id+sizeof(cm_mnbc_sound_ind.sender_id),scope);  
    ceps::interpreter::set_val("mme_cnt",cm_mnbc_sound_ind.cnt,scope);  
    ceps::interpreter::set_val("mme_rnd",cm_mnbc_sound_ind.rnd,cm_mnbc_sound_ind.rnd+sizeof(cm_mnbc_sound_ind.rnd),scope);
    return true;
}

bool mme4ceps_plugin::mme_msg_cm_atten_char_ind_setup_symtbl(homeplug_mme_generic* msg, size_t mme_size){
    cm_atten_char_ind_t& cm_atten_char_ind = msg->mmdata.cm_atten_char_ind;
    ceps::interpreter::set_val("mme_type",mme::CM_ATTEN_CHAR_IND,scope);
    ceps::interpreter::set_val("mme_application_type",cm_atten_char_ind.application_type,scope);
    ceps::interpreter::set_val("mme_security_type",cm_atten_char_ind.security_type,scope);
    ceps::interpreter::set_val("mme_run_id",cm_atten_char_ind.run_id,cm_atten_char_ind.run_id+sizeof(cm_atten_char_ind.run_id),scope);
    ceps::interpreter::set_val("mme_source_address",cm_atten_char_ind.source_address,cm_atten_char_ind.source_address+sizeof(cm_atten_char_ind.source_address),scope);  
    ceps::interpreter::set_val("mme_source_id",cm_atten_char_ind.source_id,cm_atten_char_ind.source_id+sizeof(cm_atten_char_ind.source_id),scope);  
    ceps::interpreter::set_val("mme_resp_id",cm_atten_char_ind.resp_id,cm_atten_char_ind.resp_id+sizeof(cm_atten_char_ind.resp_id),scope);  
    ceps::interpreter::set_val("mme_atten_profile",cm_atten_char_ind.atten_profile,cm_atten_char_ind.atten_profile+sizeof(cm_atten_char_ind.atten_profile),scope);  
    ceps::interpreter::set_val("mme_num_sounds",cm_atten_char_ind.num_sounds,scope);
    return true;
}

bool mme4ceps_plugin::mme_msg_cm_atten_char_rsp_setup_symtbl(homeplug_mme_generic* msg, size_t mme_size){
    cm_atten_char_rsp_t& cm_atten_char_rsp = msg->mmdata.cm_atten_char_rsp;
    ceps::interpreter::set_val("mme_type",mme::CM_ATTEN_CHAR_RSP,scope);
    ceps::interpreter::set_val("mme_application_type",cm_atten_char_rsp.application_type,scope);
    ceps::interpreter::set_val("mme_security_type",cm_atten_char_rsp.security_type,scope);
    ceps::interpreter::set_val("mme_run_id",cm_atten_char_rsp.run_id,cm_atten_char_rsp.run_id+sizeof(cm_atten_char_rsp.run_id),scope);
    ceps::interpreter::set_val("mme_source_address",cm_atten_char_rsp.source_address,cm_atten_char_rsp.source_address+sizeof(cm_atten_char_rsp.source_address),scope);  
    ceps::interpreter::set_val("mme_source_id",cm_atten_char_rsp.source_id,cm_atten_char_rsp.source_id+sizeof(cm_atten_char_rsp.source_id),scope);  
    ceps::interpreter::set_val("mme_resp_id",cm_atten_char_rsp.resp_id,cm_atten_char_rsp.resp_id+sizeof(cm_atten_char_rsp.resp_id),scope);  
    ceps::interpreter::set_val("mme_result",cm_atten_char_rsp.result,scope);
    return true;
}

bool mme4ceps_plugin::mme_msg_cm_atten_profile_ind_setup_symtbl(homeplug_mme_generic* msg, size_t mme_size){
    cm_atten_profile_ind_t& cm_atten_profile_ind = msg->mmdata.cm_atten_profile_ind;
    ceps::interpreter::set_val("mme_type",mme::CM_ATTEN_PROFILE_IND,scope);
    ceps::interpreter::set_val("mme_pev_mac",cm_atten_profile_ind.pev_mac,cm_atten_profile_ind.pev_mac+sizeof(cm_atten_profile_ind.pev_mac),scope);  
    ceps::interpreter::set_val("mme_num_groups",cm_atten_profile_ind.num_groups,scope);
    ceps::interpreter::set_val("mme_aag",cm_atten_profile_ind.aag,cm_atten_profile_ind.aag+sizeof(cm_atten_profile_ind.aag),scope);      
    return true;
}

bool mme4ceps_plugin::mme_msg_cm_validate_req_setup_symtbl(homeplug_mme_generic* msg, size_t mme_size){
    cm_validate_req_t& cm_validate_req = msg->mmdata.cm_validate_req;
    ceps::interpreter::set_val("mme_type",mme::CM_VALIDATE_REQ,scope);
    ceps::interpreter::set_val("mme_signal_type",cm_validate_req.signal_type,scope);  
    ceps::interpreter::set_val("mme_timer",cm_validate_req.timer,scope);  
    ceps::interpreter::set_val("mme_result",cm_validate_req.result,scope);  
    return true;
}

bool mme4ceps_plugin::mme_msg_cm_validate_cnf_setup_symtbl(homeplug_mme_generic* msg, size_t mme_size){
    cm_validate_cnf_t& cm_validate_cnf = msg->mmdata.cm_validate_cnf;
    ceps::interpreter::set_val("mme_type",mme::CM_VALIDATE_CNF,scope);
    ceps::interpreter::set_val("mme_signal_type",cm_validate_cnf.signal_type,scope);  
    ceps::interpreter::set_val("mme_toggle_num",cm_validate_cnf.toggle_num,scope);  
    ceps::interpreter::set_val("mme_result",cm_validate_cnf.result,scope);  
    return true;
}

bool mme4ceps_plugin::mme_msg_cm_slac_match_req_setup_symtbl(homeplug_mme_generic* msg, size_t mme_size){
    cm_slac_match_req_t& cm_slac_match_req = msg->mmdata.cm_slac_match_req;
    ceps::interpreter::set_val("mme_type",mme::CM_SLAC_MATCH_REQ,scope);
    ceps::interpreter::set_val("mme_application_type",cm_slac_match_req.application_type,scope);
    ceps::interpreter::set_val("mme_security_type",cm_slac_match_req.security_type,scope);
    ceps::interpreter::set_val("mme_run_id",cm_slac_match_req.run_id,cm_slac_match_req.run_id+sizeof(cm_slac_match_req.run_id),scope);
    ceps::interpreter::set_val("mme_mvflength",cm_slac_match_req.mvflength,scope);
    ceps::interpreter::set_val("mme_pev_id",cm_slac_match_req.pev_id,cm_slac_match_req.pev_id+sizeof(cm_slac_match_req.pev_id),scope);
    ceps::interpreter::set_val("mme_pev_mac",cm_slac_match_req.pev_mac,cm_slac_match_req.pev_mac+sizeof(cm_slac_match_req.pev_mac),scope);
    ceps::interpreter::set_val("mme_evse_id",cm_slac_match_req.evse_id,cm_slac_match_req.evse_id+sizeof(cm_slac_match_req.evse_id),scope);
    ceps::interpreter::set_val("mme_evse_mac",cm_slac_match_req.evse_mac,cm_slac_match_req.evse_mac+sizeof(cm_slac_match_req.evse_mac),scope);
    ceps::interpreter::set_val("mme_rsvd",cm_slac_match_req.rsvd,cm_slac_match_req.rsvd+sizeof(cm_slac_match_req.rsvd),scope);
    return true;
}

bool mme4ceps_plugin::mme_msg_cm_slac_match_cnf_setup_symtbl(homeplug_mme_generic* msg, size_t mme_size){
    cm_slac_match_cnf_t& cm_slac_match_cnf = msg->mmdata.cm_slac_match_cnf;
    ceps::interpreter::set_val("mme_type",mme::CM_SLAC_MATCH_CNF,scope);
    ceps::interpreter::set_val("mme_application_type",cm_slac_match_cnf.application_type,scope);
    ceps::interpreter::set_val("mme_security_type",cm_slac_match_cnf.security_type,scope);
    ceps::interpreter::set_val("mme_run_id",cm_slac_match_cnf.run_id,cm_slac_match_cnf.run_id+sizeof(cm_slac_match_cnf.run_id),scope);
    ceps::interpreter::set_val("mme_mvflength",cm_slac_match_cnf.mvflength,scope);
    ceps::interpreter::set_val("mme_pev_id",cm_slac_match_cnf.pev_id,cm_slac_match_cnf.pev_id+sizeof(cm_slac_match_cnf.pev_id),scope);
    ceps::interpreter::set_val("mme_pev_mac",cm_slac_match_cnf.pev_mac,cm_slac_match_cnf.pev_mac+sizeof(cm_slac_match_cnf.pev_mac),scope);
    ceps::interpreter::set_val("mme_evse_id",cm_slac_match_cnf.evse_id,cm_slac_match_cnf.evse_id+sizeof(cm_slac_match_cnf.evse_id),scope);
    ceps::interpreter::set_val("mme_evse_mac",cm_slac_match_cnf.evse_mac,cm_slac_match_cnf.evse_mac+sizeof(cm_slac_match_cnf.evse_mac),scope);
    ceps::interpreter::set_val("mme_nid",cm_slac_match_cnf.nid,cm_slac_match_cnf.nid+sizeof(cm_slac_match_cnf.nid),scope);
    ceps::interpreter::set_val("mme_nmk",cm_slac_match_cnf.nmk,cm_slac_match_cnf.nmk+sizeof(cm_slac_match_cnf.nmk),scope);
    ceps::interpreter::set_val("mme_rsvd",cm_slac_match_cnf.rsvd,cm_slac_match_cnf.rsvd+sizeof(cm_slac_match_cnf.rsvd),scope);
    return true;
}
  
bool mme4ceps_plugin::mme_msg_cm_set_key_req_setup_symtbl(homeplug_mme_generic* msg, size_t mme_size){
    cm_set_key_req_t& cm_set_key_req = msg->mmdata.cm_set_key_req;
    ceps::interpreter::set_val("mme_type",mme::CM_SET_KEY_REQ,scope);
    ceps::interpreter::set_val("mme_key_type",cm_set_key_req.key_type,scope);
    ceps::interpreter::set_val("mme_my_nonce",cm_set_key_req.my_nonce,cm_set_key_req.my_nonce+sizeof(cm_set_key_req.my_nonce),scope);
    ceps::interpreter::set_val("mme_your_nonce",cm_set_key_req.your_nonce,cm_set_key_req.your_nonce+sizeof(cm_set_key_req.your_nonce),scope);
    ceps::interpreter::set_val("mme_pid",cm_set_key_req.pid,scope);
    ceps::interpreter::set_val("mme_prn",cm_set_key_req.prn,scope);
    ceps::interpreter::set_val("mme_pmn",cm_set_key_req.pmn,scope);
    ceps::interpreter::set_val("mme_cco_capability",cm_set_key_req.cco_capability,scope);
    ceps::interpreter::set_val("mme_nid",cm_set_key_req.nid,cm_set_key_req.nid+sizeof(cm_set_key_req.nid),scope);
    ceps::interpreter::set_val("mme_new_eks",cm_set_key_req.new_eks,scope);
    ceps::interpreter::set_val("mme_new_key",cm_set_key_req.new_key,cm_set_key_req.new_key+sizeof(cm_set_key_req.new_key),scope);
    return true;
}

bool mme4ceps_plugin::mme_msg_cm_set_key_cnf_setup_symtbl(homeplug_mme_generic*, size_t mme_size){
    return true;
}

bool mme4ceps_plugin::mme_msg_cm_amp_map_req_setup_symtbl(homeplug_mme_generic* msg, size_t mme_size){
    cm_amp_map_req_t& cm_amp_map_req = msg->mmdata.cm_amp_map_req;
    ceps::interpreter::set_val("mme_type",mme::CM_AMP_MAP_REQ,scope);
    ceps::interpreter::set_val("mme_amlen",cm_amp_map_req.amlen,scope);
    ceps::interpreter::set_val_nibbles("mme_amdata",cm_amp_map_req.amdata,cm_amp_map_req.amdata+std::min((size_t)( (cm_amp_map_req.amlen+1)/2), 
               mme_size -  sizeof(cm_amp_map_cnf_t) + 1) ,cm_amp_map_req.amlen,scope);


    return true;
}

bool mme4ceps_plugin::mme_msg_cm_amp_map_cnf_setup_symtbl(homeplug_mme_generic* msg, size_t mme_size){
    cm_amp_map_cnf_t& cm_amp_map_cnf = msg->mmdata.cm_amp_map_cnf;
    ceps::interpreter::set_val("mme_type",mme::CM_AMP_MAP_CNF,scope);
    ceps::interpreter::set_val("mme_restype",cm_amp_map_cnf.restype,scope);

    return true;
}
