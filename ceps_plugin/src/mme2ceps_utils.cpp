#include "mme2ceps_utils.hpp"

ceps::interpreter::node_int_t ceps::interpreter::mk_val(int val){
    return ceps::interpreter::mk_int_node(val);
}

bool ceps::interpreter::set_val(void*& payload, int val){
    payload = (Nodebase_ptr) ceps::interpreter::mk_int_node(val);
    return payload;
}
