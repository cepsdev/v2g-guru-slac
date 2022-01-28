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



#include <stdlib.h>
#include <ctype.h>
#include "mme.hpp"
#include "ceps_ast.hh"

size_t write_uint16(ceps::ast::Struct_ptr strct,std::uint16_t*, Endianess); 
size_t write_int16(ceps::ast::Struct_ptr strct,std::uint16_t* pv, Endianess endianess);
size_t write_uint32(ceps::ast::Struct_ptr strct,std::uint16_t*, Endianess); 
size_t write_int32(ceps::ast::Struct_ptr strct,std::uint16_t* pv, Endianess endianess);


template<typename Iter>
size_t write_bytes(ceps::ast::Struct_ptr strct, Iter beg, Iter end){
  using namespace ceps::ast;
  auto sit = strct->children().begin();
  size_t written = 0;
  for(auto it = beg; it != end && sit != strct->children().end();++it,++sit){
    if (is<ceps::ast::Ast_node_kind::int_literal>(*sit)){
      *it = value(as_int_ref(*sit));
      ++written;
    }
    else if (is<ceps::ast::Ast_node_kind::long_literal>(*sit)){
      *it = value(as_int64_ref(*sit));
      ++written;
    }
    else if (is<ceps::ast::Ast_node_kind::float_literal>(*sit)){
      *it = value(as_double_ref(*sit));
      ++written;
    }
    else if (is<ceps::ast::Ast_node_kind::string_literal>(*sit)){
      auto const & v = value(as_string_ref(*sit));
      for(size_t i = 0; i < v.length() && it != end; ++i,++it){
        *it = v.at(i);++written;
      }
      if (it == end) break;
    }
  }
  return written;
}

template<typename C, typename Iter>
size_t write_bytes(C const & vec, Iter beg, Iter end){
  using namespace ceps::ast;
  auto sit = vec.begin();
  size_t written = 0;
  for(auto it = beg; it != end && sit != vec.end();++it,++sit){
    if (is<ceps::ast::Ast_node_kind::int_literal>(*sit)){
      *it = value(as_int_ref(*sit));
      ++written;
    }
    else if (is<ceps::ast::Ast_node_kind::long_literal>(*sit)){
      *it = value(as_int64_ref(*sit));
      ++written;
    }
    else if (is<ceps::ast::Ast_node_kind::float_literal>(*sit)){
      *it = value(as_double_ref(*sit));
      ++written;
    }
    else if (is<ceps::ast::Ast_node_kind::string_literal>(*sit)){
      auto const & v = value(as_string_ref(*sit));
      for(size_t i = 0; i < v.length() && it != end; ++i,++it){
        *it = v.at(i);++written;
      }
      if (it == end) break;
    }
  }
  return written;
}

template<typename Iter>
size_t write_nibbles(ceps::ast::Struct_ptr strct, Iter beg, Iter end){
  using namespace ceps::ast;
  auto sit = strct->children().begin();
  size_t written = 0;
  bool hnibble = false;
  for(auto it = beg; it != end && sit != strct->children().end();++sit){
    if (is<ceps::ast::Ast_node_kind::int_literal>(*sit)){
      *it = hnibble ? ( (value(as_int_ref(*sit)) << 4) | (*it & 0xF) ) : value(as_int_ref(*sit));
      ++written;
    }
    else if (is<ceps::ast::Ast_node_kind::long_literal>(*sit)){
      *it = value(as_int64_ref(*sit));
      ++written;
    }
    else if (is<ceps::ast::Ast_node_kind::float_literal>(*sit)){
      *it = value(as_double_ref(*sit));
      ++written;
    }
    else if (is<ceps::ast::Ast_node_kind::string_literal>(*sit)){
      auto const & v = value(as_string_ref(*sit));
      for(size_t i = 0; i < v.length() && it != end; ++i,++it){
        *it = v.at(i);++written;
      }
      if (it == end) break;
    }
    it+=(hnibble?1:0);
    hnibble = !hnibble;
  }
  return written;
}

size_t write(std::vector<ceps::ast::Nodebase_ptr> const & v, cm_slac_parm_req_t& msg, size_t size, Endianess);
size_t write(std::vector<ceps::ast::Nodebase_ptr> const & v, cm_slac_parm_cnf_t& msg, size_t size, Endianess);
size_t write(std::vector<ceps::ast::Nodebase_ptr> const & v, cm_start_atten_char_ind_t& msg, size_t size, Endianess);
size_t write(std::vector<ceps::ast::Nodebase_ptr> const & v, cm_atten_char_ind_t& msg, size_t size, Endianess);
size_t write(std::vector<ceps::ast::Nodebase_ptr> const & v, cm_mnbc_sound_ind_t& msg, size_t size, Endianess);
size_t write(std::vector<ceps::ast::Nodebase_ptr> const & v, cm_atten_char_rsp_t& msg, size_t size, Endianess);
size_t write(std::vector<ceps::ast::Nodebase_ptr> const & v, cm_atten_profile_ind_t& msg, size_t size, Endianess);
size_t write(std::vector<ceps::ast::Nodebase_ptr> const & v, cm_validate_req_t& msg, size_t size, Endianess);
size_t write(std::vector<ceps::ast::Nodebase_ptr> const & v, cm_validate_cnf_t& msg, size_t size, Endianess);
size_t write(std::vector<ceps::ast::Nodebase_ptr> const & v, cm_slac_match_req_t& msg, size_t size, Endianess);
size_t write(std::vector<ceps::ast::Nodebase_ptr> const & v, cm_slac_match_cnf_t& msg, size_t size, Endianess);
size_t write(std::vector<ceps::ast::Nodebase_ptr> const & v, cm_set_key_req_t& msg, size_t, Endianess);
size_t write(std::vector<ceps::ast::Nodebase_ptr> const & v, cm_amp_map_req_t& msg, size_t size, Endianess);
size_t write(std::vector<ceps::ast::Nodebase_ptr> const & v, cm_amp_map_cnf_t& msg, size_t size, Endianess);