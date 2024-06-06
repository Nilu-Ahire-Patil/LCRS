#ifndef NODE
#define NODE

#ifndef CONNECT_H
#include "include/Connect.h"	// Connect
#endif

#ifndef PACKET_H
#include "include/Packet.h"	// Packet
#endif

#ifndef NETWORK_H
#include "include/Network.h"	// Network
#endif

#include <string>		// string
#include <iostream>		// cin, cout
#include <cstdio>		// getchar

int main(int argc, char** argv)
{
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
		
		pkt.freePacketData();
	}

	getchar();
	return 0;
}	
#endif
