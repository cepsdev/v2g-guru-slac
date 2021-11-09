#include "mme4ceps.hpp"
#include "mme.hpp"

namespace tests{
 namespace handle_homeplug_mme{
    void run_all(mme4ceps_plugin& plugn);
    void cm_slac_param_req(mme4ceps_plugin& plugn);
    void cm_slac_param_cnf(mme4ceps_plugin& plugn);
    void cm_start_atten_char_ind(mme4ceps_plugin& plugn);
    void cm_mnbc_sound_ind(mme4ceps_plugin& plugn);
    void cm_atten_char_ind(mme4ceps_plugin& plugn);
    void cm_atten_profile_ind(mme4ceps_plugin& plugn);
    void cm_atten_char_rsp(mme4ceps_plugin& plugn);
    void cm_validate_req(mme4ceps_plugin& plugn);
    void cm_validate_cnf(mme4ceps_plugin& plugn);
    void cm_slac_match_req(mme4ceps_plugin& plugn);
    void cm_slac_match_cnf(mme4ceps_plugin& plugn);
    void cm_set_key_req(mme4ceps_plugin& plugn);
    void cm_set_key_cnf(mme4ceps_plugin& plugn);
    void cm_amp_map_req(mme4ceps_plugin& plugn);
    void cm_amp_map_cnf(mme4ceps_plugin& plugn);
 }
} 
