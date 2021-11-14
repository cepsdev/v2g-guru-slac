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

mme4ceps_plugin::result_t mme4ceps_plugin::start_sctp_server(std::string port){
    bool constexpr debug {false};
    addrinfo hints = {0}, *addrinfo_res = nullptr;
  
    auto sys_call_result = 0;
    if ((sys_call_result = getaddrinfo(nullptr, port.c_str(), &hints,&addrinfo_res)) != 0 )
        return result_t{false,std::string{gai_strerror(sys_call_result)}};
    
    hints.ai_flags = AI_PASSIVE | AI_NUMERICSERV;
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_SEQPACKET;
    hints.ai_protocol = IPPROTO_SCTP;

    int listenfd = -1;
    auto it = addrinfo_res;
    for(; it != nullptr && listenfd < 0; it = it->ai_next){
        if (debug){
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
        listenfd = socket(it->ai_family, SOCK_SEQPACKET, IPPROTO_SCTP);
        if (listenfd < 0) continue;
        int on {1};
        setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&on, sizeof(on));
        if (0 == bind(listenfd, it->ai_addr, it->ai_addrlen))
            break;
        close(listenfd);
        listenfd = -1;
    }//for

    if (it == nullptr)
        return {false,"Couldn't create a listening socket (socktype=SOCK_SEQPACKET, protocol=IPPROTO_SCTP)"};
    else if (debug){
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
    if (0 > listen(listenfd, 5))
     return result_t{false,"listen() failed."};

    
    std::thread reader_thread {
        [=]()
        {
            on_exit_cleanup on_exit{ [&] {if (addrinfo_res != nullptr) freeaddrinfo(addrinfo_res);}};
            socklen_t len = it->ai_addrlen;
            char readbuf[2048];
            for(;;){
                sockaddr client;
                sctp_sndrcvinfo sndrcvinfo{0};
                int msg_flags{}; 
                std::cout << "?" << std::endl;
                auto rd_sz = sctp_recvmsg(listenfd,readbuf,sizeof(readbuf),&client,&len,&sndrcvinfo,&msg_flags);
                std::cout << "!" << std::endl;
            }
        }
    };

    reader_thread.detach();

    return result_t{true,""};       
}