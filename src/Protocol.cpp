#ifndef PROTOCOL_IMPL_H
#define PROTOCOL_IMPL_H

#include "Protocol.h"		// interface

#include "Sys.h"		// Network
#include "Configure.h"		// Conf
#include "Network.h"		// Network

#include <string>	
/*-------------------------------------------------------------------------------------------------*/

// initialize tcp base packet reply
int Protocol::udpHandshakeRequest(){

	// broadcast our listening port to all connected groups and all available ports 
	unsigned short listening_port = 0;
	if((listening_port = Conf::getInfo<unsigned short>(LP)) == 0){ STOP(ERROR, ""); }

	// creating packet of listening port with port flag
	packet pkt(packetType::UdpHandshake, &listening_port, sizeof(unsigned short));

	//send listening port in broadcast
	Network nt;
	if(nt.broadcast(pkt) < 0){ STOP(ERROR, "broadcasting failed"); }

	return 0;
}

int Protocol::tcpMessage(const n_addr& addr, const std::string& msg){

	// create message packet
	packet pkt(packetType::Message,(void*) msg.c_str(), msg.length());

	// send packet using tcp
	Network nt;
	nt.sendTcpPacket(pkt, addr);
	
	return 0;
}


/*-------------------------------------------------------------------------------------------------*/
#endif

