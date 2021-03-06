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

#include "test_handle_homeplug_mme.hpp"

 void tests::handle_homeplug_mme::run_all(mme4ceps_plugin& plugn){
    cm_slac_param_req(plugn);
    cm_slac_param_cnf(plugn);
    cm_start_atten_char_ind(plugn);
    cm_mnbc_sound_ind(plugn);
    cm_atten_char_ind(plugn);
    cm_atten_profile_ind(plugn);
    cm_atten_char_rsp(plugn);
    cm_validate_req(plugn);
    cm_validate_cnf(plugn);
    cm_slac_match_req(plugn);
    cm_slac_match_cnf(plugn);
    cm_set_key_req(plugn);
    cm_set_key_cnf(plugn);
    cm_amp_map_req(plugn);
    cm_amp_map_cnf(plugn);
 }

 void tests::handle_homeplug_mme::cm_slac_param_req(mme4ceps_plugin& plugn){
   homeplug_mme_generic msg {.mmtype = mme::CM_SLAC_PARM_REQ};
   plugn.handle_homeplug_mme(&msg, sizeof(msg));
 }

 void tests::handle_homeplug_mme::cm_slac_param_cnf(mme4ceps_plugin& plugn){
   homeplug_mme_generic msg {.mmtype = mme::CM_SLAC_PARM_CNF};
   plugn.handle_homeplug_mme(&msg, sizeof(msg));
 }

void tests::handle_homeplug_mme::cm_start_atten_char_ind(mme4ceps_plugin& plugn){
   homeplug_mme_generic msg {.mmtype = mme::CM_START_ATTEN_CHAR_IND};
   plugn.handle_homeplug_mme(&msg, sizeof(msg));
}

void tests::handle_homeplug_mme::cm_mnbc_sound_ind(mme4ceps_plugin& plugn){
   homeplug_mme_generic msg {.mmtype = mme::CM_MNBC_SOUND_IND};
   plugn.handle_homeplug_mme(&msg, sizeof(msg));
}

void tests::handle_homeplug_mme::cm_atten_char_ind(mme4ceps_plugin& plugn){
  homeplug_mme_generic msg {.mmtype = mme::CM_ATTEN_CHAR_IND};
   plugn.handle_homeplug_mme(&msg, sizeof(msg));
}

void tests::handle_homeplug_mme::cm_atten_profile_ind(mme4ceps_plugin& plugn){
  homeplug_mme_generic msg {.mmtype = mme::CM_ATTEN_PROFILE_IND};
   plugn.handle_homeplug_mme(&msg, sizeof(msg));
}

void tests::handle_homeplug_mme::cm_atten_char_rsp(mme4ceps_plugin& plugn){
  homeplug_mme_generic msg {.mmtype = mme::CM_ATTEN_CHAR_RSP};
   plugn.handle_homeplug_mme(&msg, sizeof(msg));
}

void tests::handle_homeplug_mme::cm_validate_req(mme4ceps_plugin& plugn){
  homeplug_mme_generic msg {.mmtype = mme::CM_VALIDATE_REQ};
   plugn.handle_homeplug_mme(&msg, sizeof(msg));
}

void tests::handle_homeplug_mme::cm_validate_cnf(mme4ceps_plugin& plugn){
  homeplug_mme_generic msg {.mmtype = mme::CM_VALIDATE_CNF};
   plugn.handle_homeplug_mme(&msg, sizeof(msg));
}

void tests::handle_homeplug_mme::cm_slac_match_req(mme4ceps_plugin& plugn){
  homeplug_mme_generic msg {.mmtype = mme::CM_SLAC_MATCH_REQ};
   plugn.handle_homeplug_mme(&msg, sizeof(msg));
}

void tests::handle_homeplug_mme::cm_slac_match_cnf(mme4ceps_plugin& plugn){
  homeplug_mme_generic msg {.mmtype = mme::CM_SLAC_MATCH_CNF};
   plugn.handle_homeplug_mme(&msg, sizeof(msg));
}

void tests::handle_homeplug_mme::cm_set_key_req(mme4ceps_plugin& plugn){
  homeplug_mme_generic msg {.mmtype = mme::CM_SET_KEY_REQ};
   plugn.handle_homeplug_mme(&msg, sizeof(msg));
}

void tests::handle_homeplug_mme::cm_set_key_cnf(mme4ceps_plugin& plugn){
  homeplug_mme_generic msg {.mmtype = mme::CM_SET_KEY_CNF};
   plugn.handle_homeplug_mme(&msg, sizeof(msg));
}

void tests::handle_homeplug_mme::cm_amp_map_req(mme4ceps_plugin& plugn){
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

void tests::handle_homeplug_mme::cm_amp_map_cnf(mme4ceps_plugin& plugn){
  //expectation: payload{restype{11;};};
  homeplug_mme_generic msg {.mmtype = mme::CM_AMP_MAP_CNF};
  msg.mmdata.cm_amp_map_cnf.restype = 11;
  plugn.handle_homeplug_mme(&msg, sizeof(msg));
}
