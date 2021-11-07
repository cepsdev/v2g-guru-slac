#include "test_handle_homeplug_mme.hpp"

 void tests::run_all(){
    cm_slac_param_req();
    cm_slac_param_cnf();
    cm_start_atten_char_ind();
    cm_mnbc_sound_ind();
    cm_atten_char_ind();
    cm_atten_profile_ind();
    cm_atten_char_rsp();
    cm_validate_req();
    cm_validate_cnf();
    cm_slac_match_req();
    cm_slac_match_cnf();
    cm_set_key_req();
    cm_set_key_cnf();
    cm_amp_map_req();
    cm_amp_map_cnf();
 }

 void tests::cm_slac_param_req(){
   homeplug_mme_generic msg {.mmtype = mme::CM_SLAC_PARM_REQ};
   plugn.handle_homeplug_mme(&msg, sizeof(msg));
 }

 void tests::cm_slac_param_cnf(){
   homeplug_mme_generic msg {.mmtype = mme::CM_SLAC_PARM_CNF};
   plugn.handle_homeplug_mme(&msg, sizeof(msg));
 }

void tests::cm_start_atten_char_ind(){
   homeplug_mme_generic msg {.mmtype = mme::CM_START_ATTEN_CHAR_IND};
   plugn.handle_homeplug_mme(&msg, sizeof(msg));
}

void tests::cm_mnbc_sound_ind(){
   homeplug_mme_generic msg {.mmtype = mme::CM_MNBC_SOUND_IND};
   plugn.handle_homeplug_mme(&msg, sizeof(msg));
}

void tests::cm_atten_char_ind(){
  homeplug_mme_generic msg {.mmtype = mme::CM_ATTEN_CHAR_IND};
   plugn.handle_homeplug_mme(&msg, sizeof(msg));
}

void tests::cm_atten_profile_ind(){
  homeplug_mme_generic msg {.mmtype = mme::CM_ATTEN_PROFILE_IND};
   plugn.handle_homeplug_mme(&msg, sizeof(msg));
}

void tests::cm_atten_char_rsp(){
  homeplug_mme_generic msg {.mmtype = mme::CM_ATTEN_CHAR_RSP};
   plugn.handle_homeplug_mme(&msg, sizeof(msg));
}

void tests::cm_validate_req(){
  homeplug_mme_generic msg {.mmtype = mme::CM_VALIDATE_REQ};
   plugn.handle_homeplug_mme(&msg, sizeof(msg));
}

void tests::cm_validate_cnf(){
  homeplug_mme_generic msg {.mmtype = mme::CM_VALIDATE_CNF};
   plugn.handle_homeplug_mme(&msg, sizeof(msg));
}

void tests::cm_slac_match_req(){
  homeplug_mme_generic msg {.mmtype = mme::CM_SLAC_MATCH_REQ};
   plugn.handle_homeplug_mme(&msg, sizeof(msg));
}

void tests::cm_slac_match_cnf(){
  homeplug_mme_generic msg {.mmtype = mme::CM_SLAC_MATCH_CNF};
   plugn.handle_homeplug_mme(&msg, sizeof(msg));
}

void tests::cm_set_key_req(){
  homeplug_mme_generic msg {.mmtype = mme::CM_SET_KEY_REQ};
   plugn.handle_homeplug_mme(&msg, sizeof(msg));
}

void tests::cm_set_key_cnf(){
  homeplug_mme_generic msg {.mmtype = mme::CM_SET_KEY_CNF};
   plugn.handle_homeplug_mme(&msg, sizeof(msg));
}

void tests::cm_amp_map_req(){
  //expectation: payload{amlen{9;};amdata{1;2;3;4;5;6;7;8;9;};};
  homeplug_mme_generic msg {0};
  msg.mmtype = mme::CM_AMP_MAP_REQ;
  msg.mmdata.cm_amp_map_req.amlen = 9;
  msg.mmdata.cm_amp_map_req.amdata[0] = 33;
  msg.mmdata.cm_amp_map_req.amdata[1] = 67;
  msg.mmdata.cm_amp_map_req.amdata[2] = 101;
  msg.mmdata.cm_amp_map_req.amdata[3] = 135;
  msg.mmdata.cm_amp_map_req.amdata[4] = 9;
  plugn.handle_homeplug_mme(&msg, sizeof(msg));
}

void tests::cm_amp_map_cnf(){
  //expectation: payload{restype{11;};};
  homeplug_mme_generic msg {.mmtype = mme::CM_AMP_MAP_CNF};
  msg.mmdata.cm_amp_map_cnf.restype = 11;
  plugn.handle_homeplug_mme(&msg, sizeof(msg));
}
