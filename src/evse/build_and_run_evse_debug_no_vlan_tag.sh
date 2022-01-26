#!/bin/bash
g++ -I"../../include" -std=c++2a ../mme.cpp main.cpp -lsctp -lpthread -o evse && ./evse --print_mme_details --no_vlan_tag
