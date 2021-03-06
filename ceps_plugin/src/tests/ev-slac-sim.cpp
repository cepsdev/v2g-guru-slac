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
#include <unistd.h>
#include <stdlib.h>
#include <map>
#include <string.h>
#include <algorithm>
#include <netinet/sctp.h> 
#include "mme.hpp"

class EV {
    int commfd = -1;
    addrinfo * addrinfo_start; 
    addrinfo * addrinfo_used;
    public:
    EV () = default;
    EV (int commfd,addrinfo * addrinfo_start, addrinfo * addrinfo_used):commfd{commfd},addrinfo_start{addrinfo_start},addrinfo_used{addrinfo_used} {

    }
    void operator()();
    void msg_reader();
};

void EV::operator() (){
            int new_session_cmd = 0;
            auto r = sctp_sendmsg(  commfd, 
                                    &new_session_cmd,
                                    sizeof(new_session_cmd),
                                    addrinfo_used->ai_addr,addrinfo_used->ai_addrlen,0,0,0,0,0);

            sleep(50);
            std::thread reader{&EV::msg_reader,this};

            homeplug_mme_generic mme{0};
            mme.mmtype = mme::CM_SLAC_PARM_REQ;
            mme.mmdata.cm_slac_parm_req.run_id[0] = 100;
            mme.mmdata.cm_slac_parm_req.run_id[1] = 200;
            mme.mmdata.cm_slac_parm_req.run_id[2] = 201;
            

            r = sctp_sendmsg(commfd, &mme,std::max( (size_t)60,sizeof(homeplug_mme_generic_header)+sizeof(cm_slac_parm_req_t)),addrinfo_used->ai_addr,addrinfo_used->ai_addrlen,0,0,0,0,0 ) ;
            sleep(10);
            mme.mmdata.cm_slac_parm_req.run_id[0] = 101;
            mme.mmdata.cm_slac_parm_req.run_id[1] = 202;
            mme.mmdata.cm_slac_parm_req.run_id[2] = 203;

            r = sctp_sendmsg(commfd, &mme,std::max( (size_t)60,sizeof(homeplug_mme_generic_header)+sizeof(cm_slac_parm_req_t)),addrinfo_used->ai_addr,addrinfo_used->ai_addrlen,0,0,0,0,0 ) ;

            if (r <= 0){
                auto err = errno;
                std::cerr << "*** Error:sctp_sendmsg  failed: "<< strerror(err) <<"\n";
            }

            sleep(10);
            mme.mmdata.cm_slac_parm_req.run_id[0] = 104;
            mme.mmdata.cm_slac_parm_req.run_id[1] = 205;
            mme.mmdata.cm_slac_parm_req.run_id[2] = 206;

            r = sctp_sendmsg(commfd, &mme,std::max( (size_t)60,sizeof(homeplug_mme_generic_header)+sizeof(cm_slac_parm_req_t)),addrinfo_used->ai_addr,addrinfo_used->ai_addrlen,0,0,0,0,0 ) ;

            if (r <= 0){
                auto err = errno;
                std::cerr << "*** Error:sctp_sendmsg  failed: "<< strerror(err) <<"\n";
            }
            
            sleep(10);
            mme.mmdata.cm_slac_parm_req.run_id[0] = 107;
            mme.mmdata.cm_slac_parm_req.run_id[1] = 208;
            mme.mmdata.cm_slac_parm_req.run_id[2] = 209;

            r = sctp_sendmsg(commfd, &mme,std::max( (size_t)60,sizeof(homeplug_mme_generic_header)+sizeof(cm_slac_parm_req_t)),addrinfo_used->ai_addr,addrinfo_used->ai_addrlen,0,0,0,0,0 ) ;

            if (r <= 0){
                auto err = errno;
                std::cerr << "*** Error:sctp_sendmsg  failed: "<< strerror(err) <<"\n";
            }
            reader.join();    
}

void EV::msg_reader(){
    for(;;){
        sockaddr_in client {0};
        socklen_t len = sizeof(sockaddr_in);
        sctp_sndrcvinfo sndrcvinfo{0};
        memset(&client,0,sizeof(client));
        memset(&sndrcvinfo,0,sizeof(sndrcvinfo));
        int msg_flags{};
        char readbuf[2048] = {0}; 
        std::cout << "##### sctp_recvmsgsctp_recvmsgsctp_recvmsgsctp_recvmsgsctp_recvmsg "  << std::endl;
        auto rd_sz = sctp_recvmsg(commfd,readbuf,sizeof(readbuf),(sockaddr*)&client,&len,&sndrcvinfo,&msg_flags);
        std::cout << "##### READ " << rd_sz <<" bytes" << std::endl;
    }
}

int main(int argc, char** argv){
    std::string host = "localhost";
    std::string port = "56800";
    if (argc > 1) host = argv[1];
    if (argc > 2) port = argv[2];

    addrinfo hints = {0}, *addrinfo_res = nullptr; memset(&hints,0,sizeof(hints));

    hints.ai_flags = AI_NUMERICSERV;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_SEQPACKET;
    hints.ai_protocol = IPPROTO_SCTP;

    auto sys_call_result = 0;
    if ((sys_call_result = getaddrinfo(host.c_str(), port.c_str(), &hints,&addrinfo_res)) != 0 )
    {
        std::cerr << "*** Error: getaddrinfo() failed.\n";
        return 1;
    }
    
    auto connectfd = socket(AF_INET, SOCK_SEQPACKET, IPPROTO_SCTP);

    if (connectfd < 0)
    {
        std::cerr << "*** Error: socket()  failed.\n";
        return 3;
    }

    auto it = addrinfo_res;
    addrinfo* res;
    for(; it != nullptr ; it = it->ai_next){
        res = it;
        std::cout << "res->ai_addr->sa_family=" << res->ai_addr->sa_family << std::endl;
        std::cout <<  ntohs( ( (sockaddr_in*) (res->ai_addr) )->sin_port )<< std::endl;

        break;
    }//for

    if (it == nullptr)
    {
        std::cerr << "*** Error: host resolution  failed.\n";
        return 3;
    }

    sctp_event_subscribe evnts {0};memset(&evnts,0,sizeof(evnts));
    evnts.sctp_data_io_event = 1;
    setsockopt(connectfd, IPPROTO_SCTP, SCTP_EVENTS, &evnts, sizeof(evnts));


    std::thread ev {EV{connectfd,addrinfo_res,res}};
    ev.join();
    
    return 0;
}