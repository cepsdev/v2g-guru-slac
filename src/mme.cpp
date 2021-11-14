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

#include "mme.hpp"

std::ostream& operator << (std::ostream & os, homeplug_mme_generic const & mme_msg){
  os << "mme header:\n";
  os << "\t";
  os << "osa :";
  for(size_t i = 0; i < sizeof(mme_msg.osa);++i){
    os << " " << (int)mme_msg.osa[i];
  }
  os << "\n";
  os << "\t";
  os << "oda :";
  for(size_t i = 0; i < sizeof(mme_msg.oda);++i){
    os << " " << (int)mme_msg.oda[i];
  }
  os << "\n";
  os << "\t";os << "mmtype: " <<(int) mme_msg.mmtype;
  if (mme_msg.mmtype == mme::CM_SLAC_PARM_REQ)
   os << " (CM_SLAC_PARM_REQ)\n"; 
  else if (mme_msg.mmtype == mme::CM_SLAC_PARM_CNF)
   os << " (CM_SLAC_PARM_CNF)\n";
  else if (mme_msg.mmtype == mme::CM_START_ATTEN_CHAR_IND)
   os << " (CM_START_ATTEN_CHAR_IND)\n";
  else if (mme_msg.mmtype == mme::CM_MNBC_SOUND_IND)
   os << " (CM_MNBC_SOUND_IND)\n";
  else if (mme_msg.mmtype == mme::CM_ATTEN_CHAR_IND)
   os << " (CM_ATTEN_CHAR_IND)\n";
  else if (mme_msg.mmtype == mme::CM_ATTEN_CHAR_RSP)
   os << " (CM_ATTEN_CHAR_RSP)\n";
  else if (mme_msg.mmtype == mme::CM_ATTEN_PROFILE_IND)
   os << " (CM_ATTEN_PROFILE_IND)\n";
  else if (mme_msg.mmtype == mme::CM_VALIDATE_REQ)
   os << " (CM_VALIDATE_REQ)\n";
  else if (mme_msg.mmtype == mme::CM_VALIDATE_CNF)
   os << " (CM_VALIDATE_CNF)\n";
  else if (mme_msg.mmtype == mme::CM_SLAC_MATCH_REQ)
   os << " (CM_SLAC_MATCH_REQ)\n";
  else if (mme_msg.mmtype == mme::CM_SLAC_MATCH_CNF)
   os << " (CM_SLAC_MATCH_CNF)\n";
  else if (mme_msg.mmtype == mme::CM_SET_KEY_REQ)
   os << " (CM_SET_KEY_REQ)\n";
  else if (mme_msg.mmtype == mme::CM_AMP_MAP_REQ)
   os << " (CM_AMP_MAP_REQ)\n";
  else if (mme_msg.mmtype == mme::CM_AMP_MAP_CNF)
   os << " (CM_AMP_MAP_CNF)\n";
  else
   os << " (?)\n"; 
 
  os << "\t";os << "mtype: " << (int) mme_msg.mtype << "\n";
  os << "\t";os << "mmv: " << (int) mme_msg.mmv << "\n";
  os << "\t";os << "vlan_tag: " << (int) mme_msg.vlan_tag << "\n";
  os << "\t";os << "fmi: " << (int) mme_msg.fmi << "\n";
  os << "\t";os << "fmsn: " << (int) mme_msg.fmsn << "\n";
  os << "mme data:\n";
  if (mme_msg.mmtype == mme::CM_SLAC_PARM_REQ){
    os << "\t";os << "application_type: " <<(int) mme_msg.mmdata.cm_slac_parm_req.application_type << "\n";
    os << "\t";os << "security_type: " <<(int) mme_msg.mmdata.cm_slac_parm_req.security_type << "\n";
    os << "\t";os << "run_id: " ;
    for(size_t i = 0; i < sizeof(mme_msg.mmdata.cm_slac_parm_req.run_id);++i){
      os << " " << (int)mme_msg.mmdata.cm_slac_parm_req.run_id[i];
    }
    os << "\n";
  } else if (mme_msg.mmtype == mme::CM_SLAC_PARM_CNF){
    os << "\t";os << "m_sound_target: " ;
    for(size_t i = 0; i < sizeof(mme_msg.mmdata.cm_slac_parm_cnf.m_sound_target);++i){
      os << " " << (int)mme_msg.mmdata.cm_slac_parm_cnf.m_sound_target[i];
    }
    os << "\n";
    os << "\t";os << "num_sounds: " <<(int) mme_msg.mmdata.cm_slac_parm_cnf.num_sounds << "\n";
    os << "\t";os << "time_out: " <<(int) mme_msg.mmdata.cm_slac_parm_cnf.time_out << "\n";
    os << "\t";os << "resp_type: " <<(int) mme_msg.mmdata.cm_slac_parm_cnf.resp_type << "\n";
    os << "\t";os << "forwarding_sta: " ;
    for(size_t i = 0; i < sizeof(mme_msg.mmdata.cm_slac_parm_cnf.forwarding_sta);++i){
      os << " " << (int)mme_msg.mmdata.cm_slac_parm_cnf.forwarding_sta[i];
    }
    os << "\n";
    os << "\t";os << "application_type: " <<(int) mme_msg.mmdata.cm_slac_parm_cnf.application_type << "\n";
    os << "\t";os << "security_type: " <<(int) mme_msg.mmdata.cm_slac_parm_cnf.security_type << "\n";
    os << "\t";os << "run_id: " ;
    for(size_t i = 0; i < sizeof(mme_msg.mmdata.cm_slac_parm_cnf.run_id);++i){
      os << " " << (int)mme_msg.mmdata.cm_slac_parm_cnf.run_id[i];
    }
    os << "\n";
  } else if (mme_msg.mmtype == mme::CM_START_ATTEN_CHAR_IND) {
    os << "\t";os << "application_type: " <<(int) mme_msg.mmdata.cm_start_atten_char_ind.application_type << "\n";
    os << "\t";os << "security_type: " <<(int) mme_msg.mmdata.cm_start_atten_char_ind.security_type << "\n";
    os << "\t";os << "num_sounds: " <<(int) mme_msg.mmdata.cm_start_atten_char_ind.num_sounds << "\n";
    os << "\t";os << "time_out: " <<(int) mme_msg.mmdata.cm_start_atten_char_ind.time_out << "\n";
    os << "\t";os << "resp_type: " <<(int) mme_msg.mmdata.cm_start_atten_char_ind.resp_type << "\n";
    os << "\t";os << "forwarding_sta: " ;
    for(size_t i = 0; i < sizeof(mme_msg.mmdata.cm_start_atten_char_ind.forwarding_sta);++i){
      os << " " << (int)mme_msg.mmdata.cm_start_atten_char_ind.forwarding_sta[i];
    }
    os << "\n";
    os << "\t";os << "run_id: " ;
    for(size_t i = 0; i < sizeof(mme_msg.mmdata.cm_start_atten_char_ind.run_id);++i){
      os << " " << (int)mme_msg.mmdata.cm_start_atten_char_ind.run_id[i];
    }
    os << "\n";
  } else if (mme_msg.mmtype == mme::CM_MNBC_SOUND_IND) {
    os << "\t";os << "application_type: " <<(int) mme_msg.mmdata.cm_mnbc_sound_ind.application_type << "\n";
    os << "\t";os << "security_type: " <<(int) mme_msg.mmdata.cm_mnbc_sound_ind.security_type << "\n";
    os << "\t";os << "sender_id: " ;
    for(size_t i = 0; i < sizeof(mme_msg.mmdata.cm_mnbc_sound_ind.sender_id);++i){
      os << " " << (int)mme_msg.mmdata.cm_mnbc_sound_ind.sender_id[i];
    }
    os << "\n";

    os << "\t";os << "cnt: " <<(int) mme_msg.mmdata.cm_mnbc_sound_ind.cnt << "\n";
    os << "\t";os << "run_id: " ;
    for(size_t i = 0; i < sizeof(mme_msg.mmdata.cm_mnbc_sound_ind.run_id);++i){
      os << " " << (int)mme_msg.mmdata.cm_mnbc_sound_ind.run_id[i];
    }
    os << "\n";
    os << "\t";os << "rnd: " ;
    for(size_t i = 0; i < sizeof(mme_msg.mmdata.cm_mnbc_sound_ind.rnd);++i){
      os << " " << (int)mme_msg.mmdata.cm_mnbc_sound_ind.rnd[i];
    }
    os << "\n";
  } else if (mme_msg.mmtype == mme::CM_ATTEN_CHAR_IND) { 
    os << "\t";os << "application_type: " <<(int) mme_msg.mmdata.cm_atten_char_ind.application_type << "\n";
    os << "\t";os << "security_type: " <<(int) mme_msg.mmdata.cm_atten_char_ind.security_type << "\n";
    os << "\t";os << "source_address: " ;
    for(size_t i = 0; i < sizeof(mme_msg.mmdata.cm_atten_char_ind.source_address);++i){
      os << " " << (int)mme_msg.mmdata.cm_atten_char_ind.source_address[i];
    }
    os << "\n";
    os << "\t";os << "run_id: " ;
    for(size_t i = 0; i < sizeof(mme_msg.mmdata.cm_atten_char_ind.run_id);++i){
      os << " " << (int)mme_msg.mmdata.cm_atten_char_ind.run_id[i];
    }
    os << "\n";
    os << "\t";os << "source_id: " ;
    for(size_t i = 0; i < sizeof(mme_msg.mmdata.cm_atten_char_ind.source_id);++i){
      os << " " << (int)mme_msg.mmdata.cm_atten_char_ind.source_id[i];
    }
    os << "\n";
    os << "\t";os << "resp_id: " ;
    for(size_t i = 0; i < sizeof(mme_msg.mmdata.cm_atten_char_ind.resp_id);++i){
      os << " " << (int)mme_msg.mmdata.cm_atten_char_ind.resp_id[i];
    }
    os << "\n";
    os << "\t";os << "num_sounds: " <<(int) mme_msg.mmdata.cm_atten_char_ind.num_sounds << "\n";
    os << "\t";os << "atten_profile: " ;
    for(size_t i = 0; i < sizeof(mme_msg.mmdata.cm_atten_char_ind.atten_profile);++i){
      os << " " << (int)mme_msg.mmdata.cm_atten_char_ind.atten_profile[i];
    }
    os << "\n";
  } else if (mme_msg.mmtype == mme::CM_ATTEN_CHAR_RSP) { 
    os << "\t";os << "application_type: " <<(int) mme_msg.mmdata.cm_atten_char_rsp.application_type << "\n";
    os << "\t";os << "security_type: " <<(int) mme_msg.mmdata.cm_atten_char_rsp.security_type << "\n";
    os << "\t";os << "source_address: " ;
    for(size_t i = 0; i < sizeof(mme_msg.mmdata.cm_atten_char_rsp.source_address);++i){
      os << " " << (int)mme_msg.mmdata.cm_atten_char_rsp.source_address[i];
    }
    os << "\n";
    os << "\t";os << "run_id: " ;
    for(size_t i = 0; i < sizeof(mme_msg.mmdata.cm_atten_char_rsp.run_id);++i){
      os << " " << (int)mme_msg.mmdata.cm_atten_char_rsp.run_id[i];
    }
    os << "\n";
    os << "\t";os << "source_id: " ;
    for(size_t i = 0; i < sizeof(mme_msg.mmdata.cm_atten_char_rsp.source_id);++i){
      os << " " << (int)mme_msg.mmdata.cm_atten_char_rsp.source_id[i];
    }
    os << "\n";
    os << "\t";os << "resp_id: " ;
    for(size_t i = 0; i < sizeof(mme_msg.mmdata.cm_atten_char_rsp.resp_id);++i){
      os << " " << (int)mme_msg.mmdata.cm_atten_char_rsp.resp_id[i];
    }
    os << "\n";
    os << "\t";os << "result: " <<(int) mme_msg.mmdata.cm_atten_char_rsp.result << "\n";
  } else if (mme_msg.mmtype == mme::CM_ATTEN_PROFILE_IND)  { 
    os << "\t";os << "pev_mac: " ;
    for(size_t i = 0; i < sizeof(mme_msg.mmdata.cm_atten_profile_ind.pev_mac);++i){
      os << " " << (int)mme_msg.mmdata.cm_atten_profile_ind.pev_mac[i];
    }
    os << "\n";
    os << "\t";os << "num_groups: " <<(int) mme_msg.mmdata.cm_atten_profile_ind.num_groups << "\n";
    os << "\t";os << "aag: " ;
    for(size_t i = 0; i < sizeof(mme_msg.mmdata.cm_atten_profile_ind.aag);++i){
      os << " " << (int)mme_msg.mmdata.cm_atten_profile_ind.aag[i];
    }
    os << "\n";
  } else if (mme_msg.mmtype == mme::CM_VALIDATE_REQ)  { 
    os << "\t";os << "signal_type: " <<(int) mme_msg.mmdata.cm_validate_req.signal_type << "\n";
    os << "\t";os << "timer: " <<(int) mme_msg.mmdata.cm_validate_req.timer << "\n";
    os << "\t";os << "result: " <<(int) mme_msg.mmdata.cm_validate_req.result << "\n";

  } else if (mme_msg.mmtype == mme::CM_VALIDATE_CNF)  { 
    os << "\t";os << "signal_type: " <<(int) mme_msg.mmdata.cm_validate_cnf.signal_type << "\n";
    os << "\t";os << "toggle_num: " <<(int) mme_msg.mmdata.cm_validate_cnf.toggle_num << "\n";
    os << "\t";os << "result: " <<(int) mme_msg.mmdata.cm_validate_cnf.result << "\n";
  } else if (mme_msg.mmtype == mme::CM_SLAC_MATCH_REQ) { 
    os << "\t";os << "application_type: " <<(int) mme_msg.mmdata.cm_slac_match_req.application_type << "\n";
    os << "\t";os << "security_type: " <<(int) mme_msg.mmdata.cm_slac_match_req.security_type << "\n";
    os << "\t";os << "mvflength: " <<(int) mme_msg.mmdata.cm_slac_match_req.mvflength << "\n";

    os << "\t";os << "pev_id: " ;
    for(size_t i = 0; i < sizeof(mme_msg.mmdata.cm_slac_match_req.pev_id);++i){
      os << " " << (int)mme_msg.mmdata.cm_slac_match_req.pev_id[i];
    }
    os << "\n";
    os << "\t";os << "pev_mac: " ;
    for(size_t i = 0; i < sizeof(mme_msg.mmdata.cm_slac_match_req.pev_mac);++i){
      os << " " << (int)mme_msg.mmdata.cm_slac_match_req.pev_mac[i];
    }
    os << "\n";
    os << "\t";os << "evse_id: " ;
    for(size_t i = 0; i < sizeof(mme_msg.mmdata.cm_slac_match_req.evse_id);++i){
      os << " " << (int)mme_msg.mmdata.cm_slac_match_req.evse_id[i];
    }
    os << "\n";
    os << "\t";os << "evse_mac: " ;
    for(size_t i = 0; i < sizeof(mme_msg.mmdata.cm_slac_match_req.evse_mac);++i){
      os << " " << (int)mme_msg.mmdata.cm_slac_match_req.evse_mac[i];
    }
    os << "\n";
    os << "\t";os << "run_id: " ;
    for(size_t i = 0; i < sizeof(mme_msg.mmdata.cm_slac_match_req.run_id);++i){
      os << " " << (int)mme_msg.mmdata.cm_slac_match_req.run_id[i];
    }
    os << "\n";
  } else if (mme_msg.mmtype == mme::CM_SLAC_MATCH_CNF) { 
    os << "\t";os << "application_type: " <<(int) mme_msg.mmdata.cm_slac_match_cnf.application_type << "\n";
    os << "\t";os << "security_type: " <<(int) mme_msg.mmdata.cm_slac_match_cnf.security_type << "\n";
    os << "\t";os << "mvflength: " <<(int) mme_msg.mmdata.cm_slac_match_cnf.mvflength << "\n";

    os << "\t";os << "pev_id: " ;
    for(size_t i = 0; i < sizeof(mme_msg.mmdata.cm_slac_match_cnf.pev_id);++i){
      os << " " << (int)mme_msg.mmdata.cm_slac_match_cnf.pev_id[i];
    }
    os << "\n";
    os << "\t";os << "pev_mac: " ;
    for(size_t i = 0; i < sizeof(mme_msg.mmdata.cm_slac_match_cnf.pev_mac);++i){
      os << " " << (int)mme_msg.mmdata.cm_slac_match_cnf.pev_mac[i];
    }
    os << "\n";
    os << "\t";os << "evse_id: " ;
    for(size_t i = 0; i < sizeof(mme_msg.mmdata.cm_slac_match_cnf.evse_id);++i){
      os << " " << (int)mme_msg.mmdata.cm_slac_match_cnf.evse_id[i];
    }
    os << "\n";
    os << "\t";os << "evse_mac: " ;
    for(size_t i = 0; i < sizeof(mme_msg.mmdata.cm_slac_match_cnf.evse_mac);++i){
      os << " " << (int)mme_msg.mmdata.cm_slac_match_cnf.evse_mac[i];
    }
    os << "\n";
    os << "\t";os << "run_id: " ;
    for(size_t i = 0; i < sizeof(mme_msg.mmdata.cm_slac_match_cnf.run_id);++i){
      os << " " << (int)mme_msg.mmdata.cm_slac_match_cnf.run_id[i];
    }
    os << "\n";
    os << "\t";os << "nid: " ;
    for(size_t i = 0; i < sizeof(mme_msg.mmdata.cm_slac_match_cnf.nid);++i){
      os << " " << (int)mme_msg.mmdata.cm_slac_match_cnf.nid[i];
    }
    os << "\n";
    os << "\t";os << "nmk: " ;
    for(size_t i = 0; i < sizeof(mme_msg.mmdata.cm_slac_match_cnf.nmk);++i){
      os << " " << (int)mme_msg.mmdata.cm_slac_match_cnf.nmk[i];
    }
    os << "\n";
  } else if (mme_msg.mmtype == mme::CM_SET_KEY_REQ) { 
    os << "\t";os << "key_type: " <<(int) mme_msg.mmdata.cm_set_key_req.key_type << "\n";
    os << "\t";os << "my_nonce: " ;
    for(size_t i = 0; i < sizeof(mme_msg.mmdata.cm_set_key_req.my_nonce);++i){
      os << " " << (int)mme_msg.mmdata.cm_set_key_req.my_nonce[i];
    }
    os << "\n";
    os << "\t";os << "your_nonce: " ;
    for(size_t i = 0; i < sizeof(mme_msg.mmdata.cm_set_key_req.your_nonce);++i){
      os << " " << (int)mme_msg.mmdata.cm_set_key_req.your_nonce[i];
    }
    os << "\n";
    os << "\t";os << "pid: " <<(int) mme_msg.mmdata.cm_set_key_req.pid << "\n";
    os << "\t";os << "prn: " <<(int) mme_msg.mmdata.cm_set_key_req.prn << "\n";
    os << "\t";os << "pmn: " <<(int) mme_msg.mmdata.cm_set_key_req.pmn << "\n";
    os << "\t";os << "cco_capability: " <<(int) mme_msg.mmdata.cm_set_key_req.cco_capability << "\n";
    os << "\t";os << "nid: " ;
    for(size_t i = 0; i < sizeof(mme_msg.mmdata.cm_set_key_req.nid);++i){
      os << " " << (int)mme_msg.mmdata.cm_set_key_req.nid[i];
    }
    os << "\n";
    os << "\t";os << "new_eks: " <<(int) mme_msg.mmdata.cm_set_key_req.new_eks << "\n";
    os << "\t";os << "new_key: " ;
    for(size_t i = 0; i < sizeof(mme_msg.mmdata.cm_set_key_req.new_key);++i){
      os << " " << (int)mme_msg.mmdata.cm_set_key_req.new_key[i];
    }
    os << "\n";
  } else if (mme_msg.mmtype == mme::CM_AMP_MAP_REQ) {
    os << "\t";os << "amlen: " <<(int) mme_msg.mmdata.cm_amp_map_req.amlen << "\n";
    auto bytes = (size_t) (mme_msg.mmdata.cm_amp_map_req.amlen / 2);
    bool hnibble = false;
    os << "\t";
    os << "amdata: ";
    for(size_t j = 0; j < bytes; ){
      os << (int)  ( hnibble ? mme_msg.mmdata.cm_amp_map_req.amdata[j] >> 4 : mme_msg.mmdata.cm_amp_map_req.amdata[j] & 0xF)<< " ";
      j += hnibble ? 1 : 0;
      hnibble = !hnibble;
    }
    if (mme_msg.mmdata.cm_amp_map_req.amlen % 2)
      os << (int)   (mme_msg.mmdata.cm_amp_map_req.amdata[bytes] & 0xF);
    os << "\n";
  } else if (mme_msg.mmtype == mme::CM_AMP_MAP_CNF) { 
    os << "\t";os << "res_type: " <<(int) mme_msg.mmdata.cm_amp_map_cnf.restype << "\n";
  } else {
    auto p_end = (uint8_t*)&mme_msg.mmdata + sizeof(mme_msg.mmdata);
    auto ctr = 0;
    os << "\t";
    for( auto p = (uint8_t*)&mme_msg.mmdata; p != p_end; ++p){
      os << (int) *p <<  " ";
      ++ctr;
      if ( (ctr % 8) == 0) os << "\n\t";
    }
    os << "\n";
  }


  return os;
}
