#include "ceps_ast.hh"
#include "mme.hpp"
#include "core/include/state_machine_simulation_core.hpp"

using scope_t = ceps::parser_env::Scope;
using scope_ref_t = ceps::parser_env::Scope&;
using scope_ptr_t = ceps::parser_env::Scope*;

namespace ceps{
 namespace interpreter{
    ceps::interpreter::node_int_t mk_val( int val);
    bool set_val(void*& payload, int val);

    template <typename Iter> bool set_val(void*& payload, Iter from, Iter to){
        auto container = ceps::interpreter::mk_nodeset();
        payload = (Nodebase_ptr) container;
        for(auto it = from; it != to; ++it)
        container->children().push_back(mk_val(*it));
        return payload;
    }

    template <typename Iter> bool set_val_nibbles(void*& payload, Iter from, Iter to, size_t nnibbles){
        auto container = ceps::interpreter::mk_nodeset();
        payload = (Nodebase_ptr) container;
        bool hnibble = false;
        for(auto it = from; (it != to) && nnibbles;){
        container->children().push_back( hnibble ? mk_val(*it >> 4) : mk_val(*it & 0xF) );
        it += hnibble ? 1: 0;
        hnibble = !hnibble;
        --nnibbles;
        }
        return payload;
    }

 
    template <typename T> bool set_val(std::string name,T* from, T* to, scope_ref_t scope){
        auto sym = scope.insert(name);
        sym->category = ceps::parser_env::Symbol::Category::VAR;
        return set_val(sym->payload, from, to);
    }

    template <typename T> bool set_val_nibbles(std::string name,T* from, T* to, size_t nnibbles, scope_ref_t scope){
        auto sym = scope.insert(name);
        sym->category = ceps::parser_env::Symbol::Category::VAR;
        return set_val_nibbles(sym->payload, from, to,nnibbles);
    }

    template <typename T> bool set_val(std::string name,T val,scope_ref_t scope){
        auto sym = scope.insert(name);
        sym->category = ceps::parser_env::Symbol::Category::VAR;
        return set_val(sym->payload, val);
    }
 }
}
