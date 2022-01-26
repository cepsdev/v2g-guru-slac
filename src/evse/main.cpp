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
#include <optional>
#include <tuple>
#include "mme.hpp"


namespace global_settings{
    bool print_mme_details = false;
    bool encode_vlan_tag = true;
    bool big_endian = false;
}

using num_t = double;

enum  type_encoding: short{
        i32 = 5,
        i64,
        d64,
        str
    };

template <typename T>
    struct Volt{
        T v;
        Volt() = default;
        Volt(T v) : v{v} {}        
    };

template <typename T>
    struct Hz{
        T v;
        Hz() = default;
        Hz(T v) : v{v} {}        
    };

template <typename T>
    struct Percentage{
        T v;
        Percentage() = default;
        Percentage(T v) : v{v} {}        
    };


struct Oscillator{
    Hz<num_t> freq;
};

class Controlpilot{
    public:
        struct state_t{
            Volt<num_t> voltage;
            Percentage<num_t> pwm_duty_cycle;
        };
    private:
        Oscillator oscillator;
        bool oscillator_active = false;
        bool valid = false;

        std::mutex mtx;
        state_t state;
        state_t target_state;
    public:
        state_t read();
        void write(state_t s, Oscillator osci, bool osci_active);
};

Controlpilot::state_t Controlpilot::read(){
    std::lock_guard<std::mutex> lck{mtx};
    return state;
}

void Controlpilot::write(Controlpilot::state_t s, Oscillator osci, bool osci_active){
    std::lock_guard<std::mutex> lck{mtx};
    target_state = s;
    oscillator = osci;
    oscillator_active = osci_active;
}

class Evse{
    Controlpilot & controlpilot;
public:
    Evse(Controlpilot & controlpilot):controlpilot{controlpilot} {}
};

std::optional<std::pair<int,addrinfo*>> setup_sctp_connection(std::string host = "localhost", std::string port = "56800" ){

    addrinfo hints = {0}, *addrinfo_res = nullptr;
    hints.ai_flags = AI_NUMERICSERV;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_SEQPACKET;
    hints.ai_protocol = IPPROTO_SCTP;
    auto sys_call_result = 0;
    if ((sys_call_result = getaddrinfo(host.c_str(), port.c_str(), &hints,&addrinfo_res)) != 0 )
    {
        std::cerr << "*** Error: getaddrinfo() failed.\n";
        return {};
    }
    
    auto connectfd = socket(AF_INET, SOCK_SEQPACKET, IPPROTO_SCTP);

    if (connectfd < 0)
    {
        std::cerr << "*** Error: socket()  failed.\n";
        return {};
    }

    addrinfo* res = addrinfo_res;

    if (!res)
    {
        std::cerr << "*** Error: host resolution  failed.\n";
        return {};
    }

    sctp_event_subscribe evnts {0};
    evnts.sctp_data_io_event = 1;
    setsockopt(connectfd, IPPROTO_SCTP, SCTP_EVENTS, &evnts, sizeof(evnts));;
    return std::make_pair(connectfd,res);
}

void comm_handler(std::pair<int,addrinfo*> sckt_addr, Controlpilot & ctlplt){
    auto sckt = sckt_addr.first;
    auto addr = sckt_addr.second;
    
    auto hand_shake = [&](){
        int handshake_cmd = 0;
        sctp_sndrcvinfo sri = {0};
        sri.sinfo_stream = 0;
        auto r = sctp_sendmsg(sckt, &handshake_cmd,sizeof(handshake_cmd),addr->ai_addr,addr->ai_addrlen,0,0,sri.sinfo_stream,0,0 ) ;
        if (r < 0){
            auto err = errno;
            std::cerr << "*** Fatal Error:" << std::endl;
            exit(1);
        }

        for(;;){
            struct sockaddr_in peeraddr;
            constexpr int padding = 12;

            char buf[mme::max_frame_size+2*padding];
            char* mme_msg_raw;
            socklen_t len = sizeof(peeraddr);
            int msg_flags{};
            auto reply_result = sctp_recvmsg(sckt,buf+padding,mme::max_frame_size, (sockaddr*)&peeraddr,&len,&sri, &msg_flags );
            if (reply_result < 0){
                std::cerr << "*** Connection lost." << std::endl;
                exit(2);
            }
            if (!global_settings::encode_vlan_tag){
                mme_msg_raw = buf + padding - sizeof(homeplug_mme_generic::vlan_tag);
                memmove(mme_msg_raw, buf + padding, sizeof(homeplug_mme_header_mac_section));
                ((homeplug_mme_generic*)mme_msg_raw)->vlan_tag = decltype(homeplug_mme_generic::vlan_tag){};
            } else mme_msg_raw = buf + padding;
            if (global_settings::print_mme_details){
                std::cout << "***  MME received (" << reply_result << " bytes)\n";
                std::cout << *((homeplug_mme_generic*)mme_msg_raw) << std::endl; 
            }
            auto mme_msg = (homeplug_mme_generic*)mme_msg_raw;
            char out_buf[mme::max_frame_size+2*padding] = {0};
            homeplug_mme_generic& out_mme = *((homeplug_mme_generic*)(out_buf + padding));

            size_t payload_size {}; 

            if (mme_msg->mmtype == mme::CM_SLAC_PARM_REQ){
                 auto const& cm_slac_parm_req = mme_msg->mmdata.cm_slac_parm_req;
                 out_mme.mmtype = mme::CM_SLAC_PARM_CNF;
                 payload_size = sizeof(cm_slac_parm_cnf_t); 
                 auto& cm_slac_parm_cnf = out_mme.mmdata.cm_slac_parm_cnf;
                 cm_slac_parm_cnf.m_sound_target[0] = cm_slac_parm_cnf.m_sound_target[1] = cm_slac_parm_cnf.m_sound_target[2] = 
                 cm_slac_parm_cnf.m_sound_target[3] = cm_slac_parm_cnf.m_sound_target[4] = cm_slac_parm_cnf.m_sound_target[5] = 0xFF;
                 cm_slac_parm_cnf.run_id[0] = cm_slac_parm_req.run_id[0];
                 cm_slac_parm_cnf.run_id[1] = cm_slac_parm_req.run_id[1];
                 cm_slac_parm_cnf.run_id[2] = cm_slac_parm_req.run_id[2];
                 cm_slac_parm_cnf.run_id[3] = cm_slac_parm_req.run_id[3];
                 cm_slac_parm_cnf.run_id[4] = cm_slac_parm_req.run_id[4];
                 cm_slac_parm_cnf.run_id[5] = cm_slac_parm_req.run_id[5];
                 cm_slac_parm_cnf.run_id[6] = cm_slac_parm_req.run_id[6];
                 cm_slac_parm_cnf.run_id[7] = cm_slac_parm_req.run_id[7];

                    /*uint8_t m_sound_target[6];
                    uint8_t num_sounds;
                    uint8_t time_out;
                    uint8_t resp_type;
                    uint8_t forwarding_sta [6];
                    uint8_t application_type;
                    uint8_t security_type;
                    uint8_t run_id [8];
                    */
            }
            sctp_sendmsg(sckt, out_buf+padding,payload_size + sizeof(homeplug_mme_generic_header),(sockaddr*) &peeraddr,len,0,0,sri.sinfo_stream,0,0 );
        }

        struct sockaddr_in peeraddr;
        char buffer[2048];
        socklen_t len = sizeof(peeraddr);
        int msg_flags{};
        auto reply_result = sctp_recvmsg(sckt,buffer,sizeof(buffer), (sockaddr*)&peeraddr,&len,&sri, &msg_flags );
        if (reply_result == sizeof(int) && (*(int*)buffer) == 0 ){
            std::cerr << "handshake: step 1" << std::endl;

            // type name 0 | type name 0

            // msg_name (sz)
            // # entries
            // n times:
            //  type 
            //  name
            char const *  msg_name = "controlpilot";

            char const *  entry1 = "voltage";
            char const *  entry2 = "duty_cycle";
            char const *  entry3 = "oscillator_active";
            char const *  entry4 = "oscillator_freq";
            


            //send name of message
            sctp_sendmsg(sckt, msg_name,strlen(msg_name),(sockaddr*) &peeraddr,len,0,0,sri.sinfo_stream,0,0 );
            
            int entries = 4;
            sctp_sendmsg(sckt, &entries,sizeof(entries),(sockaddr*) &peeraddr,len,0,0,sri.sinfo_stream,0,0 );

            /*
            voltage double
            duty_cycle double
            oscillator_active in32
            oscillator_freq double
            */
            int t = 0;
            sctp_sendmsg(sckt, entry1,strlen(entry1),(sockaddr*) &peeraddr,len,0,0,sri.sinfo_stream,0,0 );
            t = type_encoding::d64;
            sctp_sendmsg(sckt, &t,sizeof(t),(sockaddr*) &peeraddr,len,0,0,sri.sinfo_stream,0,0 );
            
            sctp_sendmsg(sckt, entry2,strlen(entry2),(sockaddr*) &peeraddr,len,0,0,sri.sinfo_stream,0,0 );
            t = type_encoding::d64;
            sctp_sendmsg(sckt, &t,sizeof(t),(sockaddr*) &peeraddr,len,0,0,sri.sinfo_stream,0,0 );
            sctp_sendmsg(sckt, entry3,strlen(entry3),(sockaddr*) &peeraddr,len,0,0,sri.sinfo_stream,0,0 );
            t = type_encoding::i32;
            sctp_sendmsg(sckt, &t,sizeof(t),(sockaddr*) &peeraddr,len,0,0,sri.sinfo_stream,0,0 );
            sctp_sendmsg(sckt, entry4,strlen(entry4),(sockaddr*) &peeraddr,len,0,0,sri.sinfo_stream,0,0 );
            t = type_encoding::d64;
            sctp_sendmsg(sckt, &t,sizeof(t),(sockaddr*) &peeraddr,len,0,0,sri.sinfo_stream,0,0 );
            
            Hz<num_t> freq{};
            Volt<num_t> voltage{};
            Percentage<num_t> pwm_duty_cycle{};
            bool oscillator_active = false;

            for(;;){
                auto reply_result = sctp_recvmsg(sckt,buffer,sizeof(buffer), (sockaddr*)&peeraddr,&len,&sri, &msg_flags);
                if(reply_result == 4 && *((int*)buffer) == -1){ // start of message
                    for(;;){
                        reply_result = sctp_recvmsg(sckt,buffer,sizeof(buffer), (sockaddr*)&peeraddr,&len,&sri, &msg_flags);
                        if(reply_result == 4 && *((int*)buffer) == -1) {
                            ctlplt.write( Controlpilot::state_t{ voltage, pwm_duty_cycle }, Oscillator{freq}, oscillator_active  );
                            break;
                        }
                        if (*((short*) buffer) == 1){
                            voltage =  Volt<double>(*(double*)(buffer + sizeof(short)));
                        } else if (*((short*) buffer) == 2){
                            pwm_duty_cycle =  Percentage<double>(*(double*)(buffer + sizeof(short)));
                        } else if (*((short*) buffer) == 3){
                            if ( *(int*)(buffer + sizeof(short)) ) oscillator_active = true; else oscillator_active = false;
                        } else if (*((short*) buffer) == 4){
                            freq =  Hz<double>(*(double*)(buffer + sizeof(short)));
                        }
                    }
                }                 
            }
        }
        std::cout << reply_result << std::endl;
    };
    hand_shake();
} 


int main(int argc, char ** argv){
    std::string host="localhost", port="56800";
    auto read_arg_str = [&](int& i, std::string& v){
        if (i + 1 < argc){
         v = argv[i+1];
         ++i;
        }
    };

    for(int i = 0; i < argc;++i){
        std::string arg{argv[i]};
        if ( arg == "--host") read_arg_str(i,host);
        else if (arg == "--port") read_arg_str(i,port);
        else if (arg == "--no_vlan_tag") global_settings::encode_vlan_tag =  false;
        else if (arg == "--vlan_tag") global_settings::encode_vlan_tag =  true;
        else if (arg == "--print_mme_details") global_settings::print_mme_details = true;
        else if (arg == "--big_endian") global_settings::big_endian = true;
        else if (arg == "--machine_endianness") global_settings::big_endian = false;
        else if (arg == "--verbose") {
            global_settings::print_mme_details = true;
        }
    }

    auto r = setup_sctp_connection(host,port);
    if (!r.has_value()){
        std::cerr << "*** Fatal Error: Couldn't establish communication ." << std::endl;
        return 1;
    }
    Controlpilot controlpilot;
    std::thread comm_thread{comm_handler,r.value(),std::ref(controlpilot)};
    comm_thread.join();    
}