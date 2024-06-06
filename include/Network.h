#ifndef NETWORK_H
#define NETWORK_H

#ifndef ADDRBOOK_H
#include "../include/AddrBook.h"	//n_addr
#endif

#ifndef PACKET_H
#include "../include/Packet.h" 		// packet
#endif

#include <arpa/inet.h>			// sockaddr_in
					// in_addr

/*-------------------------------------------------------------------------------------------------*/

class Network {

		void processPacket(packet&, sockaddr_in&);

		int getBroadcastSocket();
		int bindUdp(int);
		int broadcast(packet&);
		int joinSingleMulticastGroup(int);
		void receveAndProcessUdp(int);

		int getTcpSocket();
		int bindAndSetTcpPort(int);
		int listenTcp(int);
		int connectTcp(int, in_addr, unsigned short);
	//	int bindTcp(int);
	

	public:

		int updateAddrSet(unsigned short);

		int setTcpListenPort();
		void receveAndProcessTcp(int);

		int sendTcpPacket(packet&, n_addr&);
		
	//	int listenTcp();
	//	int acceptTcp(int);

	//	int connectTcp(int, in_addr);
};

/*-------------------------------------------------------------------------------------------------*/

#ifndef NETWORK_IMPL_H
#include "../src/Network.cpp"
#endif

#endif

/*-------------------------------------------------------------------------------------------------*/
