#ifndef MAIN
#define MAIN

#include "Sys.h"		// getCurrentDateString
#include "Connect.h"		// Connect
#include "Packet.h"		// Packet
#include "Network.h"		// Network

#include <string>		// string
#include <iostream>		// cin, cout
#include <cstdio>		// getchar

int main(int argc, char** argv)
{
	// Redirect stderr to a file
	FILE* file = nullptr;
	std::string logFilePath = "log/" + Sys::getCurrentDateString() +".log";
	if((file = freopen(logFilePath.c_str(), "a", stderr)) == nullptr){
        	std::cerr << "Failed to redirect stderr to file" << std::endl;
	}

	if(argc < 2){ Connect cn; cn.initialize(); }
	else { Connect cn(argv[1]); cn.initialize(); }

	std::string ip;
	unsigned short port;
	std::string msg;

	Network nt;
	
	while(1){
		std::cout << "ip: ";
		std::cin >> ip;
		std::cout << "port: ";
		std::cin >> port;
		std::cout << "message: ";
		std::cin >> msg;

		packet pkt(packetType::Message,(void*) msg.c_str(), msg.length());

		n_addr addr(ip, port);

		nt.sendTcpPacket(pkt, addr);
		
	//	pkt.freePacketData();
	}

	getchar();
	return 0;
}	
#endif
