#ifndef NETWORK_H
#define NETWORK_H

#include "N_addr.h"		//n_addr
#include "Packet.h" 		// packet

#include <arpa/inet.h>		// sockaddr_in
				// in_addr

/*-------------------------------------------------------------------------------------------------*/

class Network {

		void processPacket(packet&, sockaddr_in&);

		int getBroadcastSocket();
		int bindUdp(int);
		int joinSingleMulticastGroup(int);

		int getTcpSocket();
		int bindAndSetTcpPort(int);
		int listenTcp(int);
		int connectTcp(int, in_addr, unsigned short);
	//	int bindTcp(int);
	
	//	int updateAddrSet(unsigned short);

	public:


		int setTcpListenPort();
		void receveAndProcessUdp();
		void receveAndProcessTcp(int);

		int broadcast(packet&);
		int sendTcpPacket(packet&, n_addr&);
		
	//	int listenTcp();
	//	int acceptTcp(int);

	//	int connectTcp(int, in_addr);
};

/*-------------------------------------------------------------------------------------------------*/
/*
#ifndef NETWORK_IMPL_H
#include "../src/Network.cpp"
#endif
*/
#endif

/*-------------------------------------------------------------------------------------------------*/
