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
#include <iostream>
#include <ctype.h>
#include <chrono>
#include <sstream>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <queue>
#include <unordered_map>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <stdlib.h>
#include <map>
#include <cstring>
#include <algorithm>
#include <netinet/sctp.h> 
#include "mme4ceps.hpp"
#include "mme2ceps_utils.hpp"
#include "ceps2mme.hpp"


template<typename T>
    struct on_exit_cleanup{
        T f_;
        on_exit_cleanup(T f) : f_{f} {}
        ~on_exit_cleanup() {f_();}
    };

static int create_listening_socket(std::string port,addrinfo** addr,addrinfo** addrinfo_res, bool print_debug_info){
    addrinfo hints = {0};
    memset(&hints,0,sizeof(hints));
  
    auto sys_call_result = 0;
    if ((sys_call_result = getaddrinfo(nullptr, port.c_str(), &hints,addrinfo_res)) != 0 )
        return -1;
    
    hints.ai_flags = AI_PASSIVE | AI_NUMERICSERV;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_SEQPACKET;
    hints.ai_protocol = IPPROTO_SCTP;

    int listenfd = -1;
    auto it = *addrinfo_res;
    for(; it != nullptr && listenfd < 0; it = it->ai_next){
        if (print_debug_info){
            std::cout << "# it->ai_canonname = " << (it->ai_canonname != nullptr ? it->ai_canonname : "(null)") << "\n";
            std::cout << "# it->ai_protocol = " << 
                (it->ai_protocol == IPPROTO_UDP ? "IPPROTO_UDP" : 
                (it->ai_protocol == IPPROTO_TCP ? "IPPROTO_TCP" : 
                (it->ai_protocol == IPPROTO_SCTP ? "IPPROTO_SCTP" : (it->ai_protocol == IPPROTO_ICMPV6 ? "IPPROTO_ICMPV6" : std::to_string(it->ai_protocol)) )) )  
            << "\n";       
            std::cout << "# it->ai_family = " << (it->ai_family == AF_INET6 ? "AF_INET6" : (it->ai_family == AF_INET ? "AF_INET" : (it->ai_family == AF_UNSPEC ? "AF_UNSPEC" : "?") ) )   << "\n";       
            std::cout << "# it->ai_socktype = " << it->ai_socktype  << "\n";       
            std::cout << "# it->ai_flags = " << it->ai_flags  << "\n";
        }
        listenfd = socket(AF_INET, SOCK_SEQPACKET, IPPROTO_SCTP);
        if (listenfd < 0) continue;
        int on {1};
        if (0 == bind(listenfd, it->ai_addr, it->ai_addrlen))
            break;
        close(listenfd);
        listenfd = -1;
    }//for
    *addr = it;
    return listenfd;
}

static void debug_out_listen_socket_created(int listenfd,addrinfo* it){
    std::cout << "# Listening socket: " << listenfd << "\n";
                    std::cout << "# ai_protocol = " << 
                (it->ai_protocol == IPPROTO_UDP ? "IPPROTO_UDP" : 
                (it->ai_protocol == IPPROTO_TCP ? "IPPROTO_TCP" : 
                (it->ai_protocol == IPPROTO_SCTP ? "IPPROTO_SCTP" : (it->ai_protocol == IPPROTO_ICMPV6 ? "IPPROTO_ICMPV6" : std::to_string(it->ai_protocol)) )) )  
            << "\n";       
        std::cout << "# ai_family = " << (it->ai_family == AF_INET6 ? "AF_INET6" : (it->ai_family == AF_INET ? "AF_INET" : (it->ai_family == AF_UNSPEC ? "AF_UNSPEC" : "?") ) )   << "\n";       
        std::cout << "# ai_socktype = " << it->ai_socktype  << "\n";       
        std::cout << "# ai_flags = " << it->ai_flags  << "\n";
}

mme4ceps_plugin::result_t mme4ceps_plugin::start_sctp_server(std::string port){
    bool constexpr debug {false};
    addrinfo* it{};
    addrinfo* addrinfo_res;

    auto listenfd = create_listening_socket(port, &it,&addrinfo_res, debug);

    if (listenfd < 0)
        return {false,"Couldn't create a listening socket (socktype=SOCK_SEQPACKET, protocol=IPPROTO_SCTP)"};
    else if (debug)
        debug_out_listen_socket_created(listenfd, it);

    sctp_event_subscribe evnts {0};memset(&evnts,0,sizeof(evnts));
    evnts.sctp_data_io_event = 1;
    setsockopt(listenfd, IPPROTO_SCTP, SCTP_EVENTS, &evnts, sizeof(evnts));

    if (0 > listen(listenfd, 5))
     return result_t{false,"listen() failed."};

    {
        std::lock_guard g{commfd_mtx};
        commfd = listenfd;
    }
 
    std::thread reader_thread {
        [=,this]()
        {
            on_exit_cleanup on_exit{ [&] {if (addrinfo_res != nullptr) freeaddrinfo(addrinfo_res);}};
            socklen_t len = it->ai_addrlen;
            
            constexpr unsigned int space_before = 16;
            constexpr unsigned int space_after = 16;

            char recvbuf[mme::max_frame_size + space_before + space_after];
            char * readbuf;
            // vlan_tag may be missing, we need a little space to shift the data in the right position. Rest of the
            // program assumes header with vlan_tag field. recvbuf holds the data, readbuf is a pointer which points
            // to a consolidated view of the data and serves as the mme result given to subsequentially called routines. 
            // The moving is done further below in (*)

            auto correct_buffer = [&](){
                if (encode_vlan_tag){
                    readbuf = recvbuf+space_before;
                } else {
                    // (*) Moving a little bit to make space for a default initialized vlan tag. 
                    readbuf = recvbuf+space_before - sizeof(homeplug_mme_generic::vlan_tag);
                    memmove(readbuf,recvbuf,sizeof(homeplug_mme_header_mac_section));
                    ((homeplug_mme_generic_header*)readbuf)->vlan_tag = decltype( homeplug_mme_generic::vlan_tag){};  
                }
            };

            bool debug_info{};

            for(;;){
                sockaddr_in client {0};
                len = sizeof(sockaddr_in);
                sctp_sndrcvinfo sndrcvinfo{0};
                memset(&client,0,sizeof(client));
                memset(&sndrcvinfo,0,sizeof(sndrcvinfo));
                int msg_flags{};

                auto rd_sz = sctp_recvmsg(listenfd,recvbuf+space_before,mme::max_frame_size,(sockaddr*)&client,&len,&sndrcvinfo,&msg_flags);



                if (debug_info) 
                    std::cout<< "\nread " << rd_sz <<" bytes info_stream =" << sndrcvinfo.sinfo_stream << "\n";                        

                if (rd_sz > 0 && sndrcvinfo.sinfo_stream != 0){
                    auto ch = sndrcvinfo.sinfo_stream;
                    auto read_buf = recvbuf+space_before;
                    int handshake_cmd = 0;
                    
                    std::lock_guard g{commfd_mtx};
                    auto& channel = channels[ch];

                    if ( (channel.status == Channel::Status::Established || channel.status == Channel::Status::NotInitialized) && 
                          (rd_sz == sizeof(handshake_cmd)) && (*(( decltype(handshake_cmd) *)read_buf) == handshake_cmd) ) {
                        //New Message handshake
                        channel.status = Channel::Status::WaitForMessageName;
                        //Send Ack
                        sctp_sendmsg(listenfd, &handshake_cmd,sizeof(handshake_cmd),(sockaddr*) &client,len,0,0,sndrcvinfo.sinfo_stream,0,0);
                        channel.message = {};
                        channel.chidx = sndrcvinfo.sinfo_stream;
                        if (debug_info) 
                            std::cout<< "\nread " << rd_sz <<" bytes info_stream =" << sndrcvinfo.sinfo_stream << ", Status changed to Channel::Status::WaitForMessageName" << "\n";                        
                    } else if (channel.status == Channel::Status::WaitForMessageName) {
                        if (debug_info) 
                            std::cout<< "\nread " << rd_sz <<" bytes info_stream =" << sndrcvinfo.sinfo_stream << " Status is Channel::Status::WaitForMessageName." << "\n";
                        *(read_buf+rd_sz) = 0;
                        channel.message.name = read_buf;
                        channel.status = Channel::Status::WaitForNumberOfMessageEntries;
                        if (debug_info) 
                            std::cout<< "Message handshake: (info_stream = " << sndrcvinfo.sinfo_stream <<") message name = "<< channel.message.name << "\n";                        
                    } else if (channel.status == Channel::Status::WaitForNumberOfMessageEntries && rd_sz == sizeof(int)) {
                        if (debug_info) 
                            std::cout<< "\nread " << rd_sz <<" bytes info_stream =" << sndrcvinfo.sinfo_stream << " Status is Channel::Status::WaitForMessageName." << "\n";
                        channel.remaining_field_entries = *((int*) read_buf);
                        channel.status = Channel::Status::WaitForType;
                        if (debug_info) 
                            std::cout<< "remaining_field_entries=" << channel.remaining_field_entries << "\n";
                    } else if ( channel.status == Channel::Status::WaitForType){
                        auto type_read = *((int*)read_buf);
                        channel.message.fields.push_back({"", (Channel::type_encoding)type_read});
                        channel.status = Channel::Status::WaitForFieldName;
                        if (debug_info) 
                            std::cout<< "field type =" << type_read << "\n";
                    } else if (channel.status == Channel::Status::WaitForFieldName){
                        read_buf[rd_sz] = 0;
                        channel.message.fields[channel.message.fields.size()-1].name = read_buf;
                        if (debug_info)
                            std::cout << "field name=" << channel.message.fields[channel.message.fields.size()-1].name << " remaining fields: " << (channel.remaining_field_entries-1) << "\n";
                        --channel.remaining_field_entries;
                        if (channel.remaining_field_entries > 0) 
                            channel.status = Channel::Status::WaitForType;
                        else {
                            channel.status = Channel::Status::Established;  
                            if (debug_info){                                
                                std::cout << "Message channel established(channel id=" << channel.chidx << ")\n";
                                std::cout << "\tMessage = " << channel.message.name << " {\n";
                                for (auto const & e: channel.message.fields){
                                    std::cout << "\t"<< e.name << "; // type =";
                                    if (e.type == Channel::type_encoding::d64) std::cout << "double";
                                    else if (e.type == Channel::type_encoding::i32) std::cout << "int";
                                    if (e.type == Channel::type_encoding::i64) std::cout << "long long";
                                    if (e.type == Channel::type_encoding::str) std::cout << "string";
                                    std::cout << "\n";
                                }
                                std::cout << "};\n";
                            }
                        }
                    }
                } else if(rd_sz > 0) {
                    correct_buffer();
                    bool new_client = false;
                    {
                        std::lock_guard g{commfd_mtx};
                        new_client = client.sin_port != last_client.sin_port || client.sin_addr.s_addr != last_client.sin_addr.s_addr;
                        last_client = client;
                        last_client_len = len;
                    } 
                    if (new_client && !on_client_connect.empty())
                      execute_ceps(on_client_connect);


                    if (encode_big_endianness) {
                        cov_to_machine_endianness( *((homeplug_mme_generic*) readbuf) , Endianness::big);
                    }
                    this->handle_homeplug_mme( (homeplug_mme_generic*) readbuf,rd_sz);
                }
            }
        }
    };

    reader_thread.detach();

    return result_t{true,""};       
}