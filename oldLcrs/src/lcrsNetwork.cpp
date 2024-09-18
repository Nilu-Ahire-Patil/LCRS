#include "lcrsNetwork.h"

LCRS_NAMESPACE

lcrsNetwork::lcrsNetwork(const uint8_t socketFlags)
: mSocketFlags(socketFlags), mDomain(AF_INET), mProtocol(0)
{
    mType = SOCK_DGRAM;

    if(EXIT_FAILURE == InitSocket()){ SPDLOG_DEBUG("lcrsNetwork fail to initilise socket"); }
}

bool lcrsNetwork::InitSocket(){
    if(SocketFlags::UNINITILISED == mSocketFlags) { 
        SPDLOG_DEBUG("InitSocket fail to initilise uninitilised type socket"); 
        return EXIT_FAILURE;
    }
    
    if(SocketType::UDP & mSocketFlags){
        mType = SOCK_DGRAM;
        if((mSocket = socket(mDomain, mType, 0)) < 0) { 
            SPDLOG_DEBUG("InitSocket fail to Add socket opction {}", (int) SocketType::UDP);
            return EXIT_FAILURE;
        }
        SPDLOG_TRACE("Added {} opction in socket {}", (int) SocketType::UDP, mSocket);
    }

    if(SocketType::TCP & mSocketFlags){
        if((mSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0){ 
            SPDLOG_DEBUG("InitSocket fail to Add socket opction {}", (int) SocketType::TCP);
            return EXIT_FAILURE;
        }
        SPDLOG_TRACE("Added {} opction in socket {}", (int) SocketType::TCP, mSocket);
    }

    if(SocketType::BROADCAST & mSocketFlags){
        int broadcast_flag = 1;
        if(setsockopt(mSocket, SOL_SOCKET, SO_BROADCAST, (char*)&broadcast_flag, sizeof(broadcast_flag)) < 0){ 
            SPDLOG_DEBUG("InitSocket fail to Add socket opction {}", (int) SocketType::BROADCAST);
            return EXIT_FAILURE;
        }
        SPDLOG_TRACE("Added {} opction in socket {}", (int) SocketType::BROADCAST, mSocket);
    }

    if(SocketType::MULTICAST & mSocketFlags){
        int TTL = 99;
        if(setsockopt(soc, IPPROTO_IP, IP_MULTICAST_TTL, (char*)&TTL , sizeof(TTL)) < 0){ 
             SPDLOG_DEBUG("InitSocket fail to Add socket opction {}", (int) SocketType::MULTICAST);
            return EXIT_FAILURE;
        }
        SPDLOG_TRACE("Added {} opction in socket {}", (int) SocketType::MULTICAST, mSocket);
    }

    if(SocketType::REUSEADDR & mSocketFlags){
        int reuseAddr = 1;
	    if(setsockopt(soc, SOL_SOCKET, SO_REUSEADDR, &reuseAddr, sizeof(optval)) < 0){ 
            SPDLOG_DEBUG("InitSocket fail to Add socket opction {}", (int) SocketType::REUSEADDR);
            return EXIT_FAILURE;
        }
        SPDLOG_TRACE("Added {} opction in socket {}", (int) SocketType::REUSEADDR, mSocket);
    }

    if(SocketType::SNDTIMEO & mSocketFlags){
        struct timeval timeout;
    	timeout.tv_sec = 2; // seconds
    	timeout.tv_usec = 0;
	    iif(setsockopt(soc, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout)) < 0){ 
            SPDLOG_DEBUG("InitSocket fail to Add socket opction {}", (int) SocketType::SNDTIMEO);
            return EXIT_FAILURE;
        }
        SPDLOG_TRACE("Added {} opction in socket {}", (int) SocketType::SNDTIMEO, mSocket);
    }

    return EXIT_SUCCESS;
}