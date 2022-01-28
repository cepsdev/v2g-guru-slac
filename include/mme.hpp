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
#pragma once

#include <ctype.h>
#include <sys/types.h>
#include <stdint.h>
#include <ostream>

namespace mme{
      uint16_t constexpr REQ = 0;
      uint16_t constexpr CNF = 1;
      uint16_t constexpr IND = 2;
      uint16_t constexpr RSP = 3;

      inline auto is_request(uint16_t msg) { return (msg & 3) == REQ;}
      inline auto is_confirmation(uint16_t msg) { return (msg & 3) == CNF;}
      inline auto is_indication(uint16_t msg) { return (msg & 3) == IND;}
      inline auto is_response(uint16_t msg) { return (msg & 3) == RSP;}


      uint16_t constexpr CM_SLAC_PARM_BASE = 0x6064;
      uint16_t constexpr CM_SET_KEY_BASE = 0x6008;
      uint16_t constexpr CM_AMP_MAP_BASE = 0x601C;
      uint16_t constexpr CM_START_ATTEN_CHAR_BASE = 0x6068;
      uint16_t constexpr CM_ATTEN_CHAR_BASE = 0x606C;
      uint16_t constexpr CM_MNBC_SOUND_BASE = 0x6074;
      uint16_t constexpr CM_VALIDATE_BASE = 0x6078;
      uint16_t constexpr CM_SLAC_MATCH_BASE = 0x607C;
      uint16_t constexpr CM_ATTEN_PROFILE_BASE = 0x6084;

      uint16_t constexpr CM_SLAC_PARM_REQ = CM_SLAC_PARM_BASE | REQ;
      uint16_t constexpr CM_SLAC_PARM_CNF = CM_SLAC_PARM_BASE | CNF;
      uint16_t constexpr CM_START_ATTEN_CHAR_IND = CM_START_ATTEN_CHAR_BASE | IND;
      uint16_t constexpr CM_MNBC_SOUND_IND = CM_MNBC_SOUND_BASE | IND;
      uint16_t constexpr CM_ATTEN_CHAR_IND = CM_ATTEN_CHAR_BASE | IND;
      uint16_t constexpr CM_ATTEN_PROFILE_IND = CM_ATTEN_PROFILE_BASE | IND;
      uint16_t constexpr CM_ATTEN_CHAR_RSP = CM_ATTEN_CHAR_BASE | RSP;
      uint16_t constexpr CM_VALIDATE_REQ = CM_VALIDATE_BASE | REQ;
      uint16_t constexpr CM_VALIDATE_CNF =  CM_VALIDATE_BASE | CNF;
      uint16_t constexpr CM_SLAC_MATCH_REQ = CM_SLAC_MATCH_BASE | REQ;
      uint16_t constexpr CM_SLAC_MATCH_CNF = CM_SLAC_MATCH_BASE | CNF;
      uint16_t constexpr CM_SET_KEY_REQ = CM_SET_KEY_BASE | REQ;
      uint16_t constexpr CM_SET_KEY_CNF =  CM_SET_KEY_BASE | CNF;
      uint16_t constexpr CM_AMP_MAP_REQ = CM_AMP_MAP_BASE | REQ;
      uint16_t constexpr CM_AMP_MAP_CNF = CM_AMP_MAP_BASE | CNF;

      unsigned int constexpr max_frame_size = 2048;
}

struct __attribute__((__packed__))  cm_slac_parm_req_t {
      uint8_t application_type;
      uint8_t security_type;
      uint8_t run_id [8];
};

struct __attribute__((__packed__))  cm_slac_parm_cnf_t {
      uint8_t m_sound_target[6];
      uint8_t num_sounds;
      uint8_t time_out;
      uint8_t resp_type;
      uint8_t forwarding_sta [6];
      uint8_t application_type;
      uint8_t security_type;
      uint8_t run_id [8];
};

struct __attribute__((__packed__))  cm_start_atten_char_ind_t{
      uint8_t application_type;
      uint8_t security_type;
      uint8_t num_sounds;
      uint8_t time_out;
      uint8_t resp_type;
      uint8_t forwarding_sta [6];
      uint8_t run_id [8];
};

struct __attribute__((__packed__))  cm_mnbc_sound_ind_t{
      uint8_t application_type;
      uint8_t security_type;
      uint8_t sender_id[17];
      uint8_t cnt;
      uint8_t run_id [8];
      uint8_t reserved[8];
      uint8_t rnd[16];
};

struct __attribute__((__packed__))  cm_atten_char_ind_t{
      uint8_t application_type;
      uint8_t security_type;
      uint8_t source_address[6];
      uint8_t run_id [8];
      uint8_t source_id[17];
      uint8_t resp_id[17];
      uint8_t num_sounds;
      uint8_t atten_profile[59];
};

struct __attribute__((__packed__))  cm_atten_char_rsp_t{
      uint8_t application_type;
      uint8_t security_type;
      uint8_t source_address[6];
      uint8_t run_id [8];
      uint8_t source_id[17];
      uint8_t resp_id[17];
      uint8_t result;
};

struct __attribute__((__packed__))  cm_atten_profile_ind_t{
      uint8_t pev_mac[6];
      uint8_t num_groups;
      uint8_t rsvd;
      uint8_t aag[58];
};

struct __attribute__((__packed__))  cm_validate_req_t{
      uint8_t signal_type;
      uint8_t timer;
      uint8_t result;
};

struct __attribute__((__packed__))  cm_validate_cnf_t{
      uint8_t signal_type;
      uint8_t toggle_num;
      uint8_t result;
};

struct __attribute__((__packed__))  cm_slac_match_req_t{
      uint8_t application_type;
      uint8_t security_type;
      uint16_t mvflength;
      uint8_t pev_id[17];
      uint8_t pev_mac[6];
      uint8_t evse_id[17];
      uint8_t evse_mac[6];
      uint8_t run_id [8];
      uint8_t rsvd [8];
};

struct __attribute__((__packed__))  cm_slac_match_cnf_t{
      uint8_t application_type;
      uint8_t security_type;
      uint16_t mvflength;
      uint8_t pev_id[17];
      uint8_t pev_mac[6];
      uint8_t evse_id[17];
      uint8_t evse_mac[6];
      uint8_t run_id [8];
      uint8_t rsvd [8];
      uint8_t nid[7];
      uint8_t rsvd2;
      uint8_t nmk[16];
};

struct __attribute__((__packed__))  cm_set_key_req_t{
      uint8_t key_type;
      uint8_t my_nonce[4];
      uint8_t your_nonce[4];
      uint8_t pid;
      uint16_t prn;
      uint8_t pmn;
      uint8_t cco_capability;
      uint8_t nid[7];
      uint8_t new_eks;
      uint8_t new_key[16];
};

struct __attribute__((__packed__))  cm_amp_map_req_t{
      uint16_t amlen;
      uint8_t amdata[1];
};

struct __attribute__((__packed__))  cm_amp_map_cnf_t{
      uint8_t restype;
};    

struct __attribute__((__packed__))  homeplug_mme_generic_header{
  char oda[6];
  char osa[6];
  uint32_t vlan_tag;
  uint16_t mtype;
  uint8_t mmv;
  uint16_t mmtype;
  uint8_t fmi;
  uint8_t fmsn;
};

struct __attribute__((__packed__))  homeplug_mme_header_mac_section{
  char oda[6];
  char osa[6];
};

struct __attribute__((__packed__))  homeplug_mme_generic{
  char oda[6];
  char osa[6];
  uint32_t vlan_tag;
  uint16_t mtype;
  uint8_t mmv;
  uint16_t mmtype;
  uint8_t fmi;
  uint8_t fmsn;
  union{
    cm_slac_parm_req_t cm_slac_parm_req;
    cm_slac_parm_cnf_t cm_slac_parm_cnf;
    cm_start_atten_char_ind_t cm_start_atten_char_ind;
    cm_mnbc_sound_ind_t cm_mnbc_sound_ind;
    cm_atten_char_ind_t cm_atten_char_ind;
    cm_atten_char_rsp_t cm_atten_char_rsp;
    cm_atten_profile_ind_t cm_atten_profile_ind;
    cm_validate_req_t cm_validate_req;
    cm_validate_cnf_t cm_validate_cnf;
    cm_slac_match_req_t cm_slac_match_req;
    cm_slac_match_cnf_t cm_slac_match_cnf;
    cm_set_key_req_t cm_set_key_req;
    cm_amp_map_req_t cm_amp_map_req;
    cm_amp_map_cnf_t cm_amp_map_cnf;    
    struct{

    } cm_set_key_cnf;
  } mmdata;
};

std::ostream& operator << (std::ostream & , homeplug_mme_generic const & );

enum class Endianess {machine,big};

template <Endianess endianess, typename T1, typename T2>
 inline T1 cov_endianess_narrow(T2 v) {
        return v;
  }

template <>
  std::uint16_t cov_endianess_narrow<Endianess::big,std::uint16_t,int>(int v);
template <>
  std::int16_t cov_endianess_narrow<Endianess::big,std::int16_t,int>(int v);
template <>
  std::uint32_t cov_endianess_narrow<Endianess::big,std::uint32_t,int>(int v);
template <>
  std::int32_t cov_endianess_narrow<Endianess::big,std::int32_t,int>(int v);



