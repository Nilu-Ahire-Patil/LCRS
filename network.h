#pragma once

//#include <arpa/inet.h>
//#include <set>
//#include <iostream>
//#include <cstring>
//#include <unistd.h>
#include <thread>
#include "conf.h"

using namespace std;

class Network : public Conf {

		int getBroadcastSocket();
		int bindUdp(int);
		int broadcast(packet&);
		int joinSingleMulticastGroup(int);
		void receveAndProcessUdp(int);

		int getTcpSocket();
		int bindAndSetTcpPort(int);
		int listenTcp(int);
	//	int bindTcp();
	
		void processPacket(packet, sockaddr_in);

	public:

		int updateAddrSet(unsigned short);

		int setTcpListenPort();
		void receveAndProcessTcp(int);
		
	//	int listenTcp();
	//	int acceptTcp(int);

	//	int connectTcp(int, in_addr);
	//	int connectTcp(int, in_addr, unsigned short);
};

/*-------------------------------------------------------------------------------------------------*/

// returns an socket with broadcasting permissions
int Network :: getBroadcastSocket(){
	// create udp socket
	int soc = -1;
	if((soc = socket(AF_INET, SOCK_DGRAM, 0)) < 0){ 
		STOP(ERROR, "socket not set properly"); 
	}
	
	// set broadcast option to socket
	int broadcast_flag = 1;
	if(setsockopt(soc, SOL_SOCKET, SO_BROADCAST, (char*)&broadcast_flag, sizeof(broadcast_flag)) < 0){ 
		STOP(ERROR, "fail to set broadcast option"); 
	}

	// set time to leave value for routing the packet
	int TTL = 99;
	if(setsockopt(soc, IPPROTO_IP, IP_MULTICAST_TTL, (char*)&TTL , sizeof(TTL)) < 0){ 
		STOP(ERROR, "fail to set multicast option"); 
	}

	return soc;
}

// binding an udp socket to available port
int Network :: bindUdp(int soc){
	if(soc < 0){ 
		STOP(ERROR, "socket not set properly"); 
	}
	
       	// get udp port's	
	set<int> ports = getInfo<set<int>>(UDP_PORTS);

	if(ports.empty()){ 
		STOP(ERROR, "fail to find udp port's"); 
	}

	// try to bind socket with port one bye one
	set<int>::iterator it = ports.begin();
	while(it != ports.end()){
		sockaddr_in add;
		memset(&add, 0, sizeof(sockaddr_in));
		add.sin_family = AF_INET;
		add.sin_port = htons(*it);
		add.sin_addr.s_addr = INADDR_ANY;

		// try to bind port
		if(bind(soc, (sockaddr*) &add, sizeof(sockaddr_in)) < 0){ 
			SYSLOG(WARN, std::to_string(*it) + ":fail to bind port"); 
			++it; 
		}
		else { 
			SYSLOG(SUCCESS, std::to_string(*it) + ":UDP port configure:" ); 
			break; 
		}
	}
	if(it == ports.end()){ 
		STOP(ERROR, "all udp port's already used"); 
	}

	return soc;
}

// broadcast packet to all available groups and ports
int Network :: broadcast(packet& pkt){
	int soc = -1;
	if((soc = getBroadcastSocket()) < 0){ 
		STOP(ERROR, "socket not set properly"); 
	}

	// serialise packet for sending
	char* buffer = pkt.serialize();

	// other node address
	struct sockaddr_in oth_addr = { 0 };
    	oth_addr.sin_family = AF_INET;

	// get multicast group addresses
	set<string> bga = getInfo<set<string>>(BGA);

	if(bga.empty()){ 
		STOP(ERROR, "no multicast group found"); 
	}

	// send packet to all available multicast group's
	set<string>::iterator it = bga.begin(); 
	while(it != bga.end()){
		oth_addr.sin_addr.s_addr = inet_addr((*it).c_str()); //broadcast Group Address

       		// get udp port's	
		set<int> ports = getInfo<set<int>>(UDP_PORTS);

		if(ports.empty()){ STOP(ERROR, "no udp port found"); }

		// send packet to all available port,s
		set<int>::iterator pit = ports.begin(); 
		while(pit != ports.end()){
			//set port
    			oth_addr.sin_port = htons(*pit);

			// broadcast packet
			int s_byte;
    			if((s_byte = sendto(soc, buffer, pkt.getFullSize(), 0, (sockaddr*)&oth_addr, sizeof(sockaddr_in))) < 0){ 
				SYSLOG(WARN, *it + ":" + std::to_string(*pit) + ":sending packet fail"); 
			}
			++pit;
		}
		++it;
    	}

	SYSLOG(SUCCESS, "broadcast sent complete");

	// close broadcasting socket
	close(soc);

	return 0;
}

// join an udp socket to one available multicast group
int Network :: joinSingleMulticastGroup(int soc){
	if(soc < 0){ 
		STOP(ERROR, "socket not set properly"); 
	}

	ip_mreq mreq;
	// connect any available ip
	mreq.imr_interface.s_addr = htonl(INADDR_ANY);

	// get multicast group addresses
	set<string> bga = getInfo<set<string>>(BGA);

	if(bga.empty()){ STOP(ERROR, "no multicast group found"); }

	// try to join one multicast group
	set<string>::iterator it = bga.begin(); 
	while(it != bga.end()){
		// Multicast group address
		mreq.imr_multiaddr.s_addr = inet_addr((*it).c_str()); 	

		// joining the multicast group
		if(setsockopt(soc, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&mreq, sizeof(mreq)) < 0){ 
			SYSLOG(WARN, *it + ":fail to join multicast group"); 
		}
		else{ 
			SYSLOG(SUCCESS, *it + ":multicast group join"); 
			return soc; 
		} 
		++it;
	}
	if(it == bga.end()){ 
		STOP(WARN, "running out of multicast groups"); 
	}

	return -1;
}

// receive an packet and take action according its type
void Network :: receveAndProcessUdp(int soc){
	if(soc < 0){ 
		STOP(ERROR, "socket not set properly"); 
	}

	// sender address
    	sockaddr_in sender_addr = { 0 };
	socklen_t sender_addr_len = sizeof(sender_addr);

	while(1){
		// message buffer
		char buffer[MAX_PACKET_SIZE] = { '\0' };
		int recv_byte;
    		if((recv_byte = recvfrom(soc, buffer, MAX_PACKET_SIZE, 0, (sockaddr*) &sender_addr, &sender_addr_len)) < 0){ 
			SYSLOG(WARN, "fail to receive packet"); 
			continue; 
		}

		packet pkt;
		// convert received message to buffer
		pkt.deserialize(buffer);
		// check packet is complete or not
		if(recv_byte != sizeof(packetHeader) + pkt.getSize()){ 
			SYSLOG(WARN, "incomplete packet found"); 
			continue; 
		}
		SYSLOG(INFO, string(inet_ntoa(sender_addr.sin_addr)) + " : " + packetTypeToString(pkt.getType()) + " : " + to_string(recv_byte) + " Bytes");

		// check the packet and take action according its type 
		processPacket(pkt, sender_addr);

		// free packet buffer data;
		pkt.freePacketData();
	}
    	close(soc);
}

// takes an packet as input and process according that packet type
void Network :: processPacket(packet pkt, sockaddr_in sender_addr){
	switch(pkt.getType()){
		case packetType :: UdpHandshake : Conf :: a_book.add(sender_addr.sin_addr,*(unsigned short*)pkt.data); break;
		case packetType :: Unknown : SYSLOG(INFO, "unknown packet to process"); break;
		default : SYSLOG(INFO, "packet type can't identified");
	}
}

// broadcast our listening port to all connected groups and all available ports 
int Network :: updateAddrSet(unsigned short listening_port){
	//creating packet of listening port with port flag
	packet pkt;
	//initialise packet;
	pkt.init(packetType :: UdpHandshake, &listening_port, sizeof(unsigned short));
	//send listening port in broadcast
	if(broadcast(pkt) < 0){ 
		STOP(ERROR, "broadcasting failed"); 
	}
	//free reserve memory used for serializing the packet
	pkt.freePacketData();
	//accepting broadcast
	
	int soc = joinSingleMulticastGroup(bindUdp(getBroadcastSocket()));

	//replying broadcast
        thread udpReplying([this, soc]() { receveAndProcessUdp(soc); });
	udpReplying.detach();
	
	return soc;
}

/*-------------------------------------------------------------------------------------------------*/

// returns an tcp socket
int Network :: getTcpSocket(){
	// get tcp socket
	int soc = -1;
	if((soc = socket(AF_INET, SOCK_STREAM, 0)) < 0){ 
		STOP(ERROR, "socket not set properly"); 
	}

	// set socket re usability option
	int optval = 1;
	if(setsockopt(soc, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0){ 
		SYSLOG(WARN, "fail to set socket address re usability"); 
	}

	return soc;
}

// bind tcp socket with available port and sets listening_port according to it
int Network :: bindAndSetTcpPort(int soc){
	if(soc < 0){ 
		STOP(ERROR, "socket not set properly"); 
	}

	// self address
	sockaddr_in add = { 0 };
	add.sin_family = AF_INET;
	add.sin_addr.s_addr = INADDR_ANY;
 
	// get tcp port's
	set<int> ports = getInfo<set<int>>(TCP_PORTS);

	if(ports.empty()){ STOP(ERROR, ""); }

	// try to bind and set tcp port
	set<int>::iterator it = ports.begin();
	while(it != ports.end()){
		add.sin_port = htons(*it);

		// trying to bind port
		if(bind(soc, (sockaddr*) &add, sizeof(sockaddr_in)) < 0){ 
			SYSLOG(WARN, std::to_string(*it) + ":fail to bind port"); 
			++it;
		}
		else{
			Conf :: setInfo<unsigned short>(LP, (unsigned short)*it);
			SYSLOG(SUCCESS, to_string(*it) + ":tcp port configure" );
			break;
		}
	}
	if(it == ports.end()){ 
		STOP(ERROR, "running out of tcp port"); 
	}

	return soc;
}

// starting listening on tcp socket
int Network :: listenTcp(int soc){
	if(soc < 0){ 
		STOP(ERROR, "socket not set properly"); 
	}
	
	// start listening on tcp socket
	if(listen(soc, getInfo<int>(MCB)) < 0){ 
		STOP(ERROR, "fail listening on socket"); 
	}

	return soc;
}

// receive client and processes their packets
void Network :: receveAndProcessTcp(int soc){
	if(soc < 0){ STOP(ERROR, "socket not set properly"); }

	// sender address
    	sockaddr_in sender_addr = { 0 };
	socklen_t sender_addr_len = sizeof(sender_addr);

	while(1){
		// accepting all incoming connections
		int csoc = -1;
		if((csoc = accept(soc, (struct sockaddr*) &sender_addr, &sender_addr_len)) < 0){ 
			SYSLOG(WARN, "tcp accept fail");
			continue; 
		}

		SYSLOG(INFO, "new connection accept");

		// receive packet 
		char buffer[MAX_PACKET_SIZE] = { '\0' };
		int recv_byte;
    		if((recv_byte = recv(csoc, buffer, MAX_PACKET_SIZE, 0)) < 0){ 
			SYSLOG(WARN, "fail to accept message"); 
			continue; 
		}

		// we can send acknowledgement 

		// close connection with sender
		close(csoc);

		packet pkt;
		// convert received message to buffer
		pkt.deserialize(buffer);
		// check packet is complete or not
		if(recv_byte != sizeof(packetHeader) + pkt.getSize()){ 
			SYSLOG(INFO, "incomplete packet found"); 
			continue; 
		}
		SYSLOG(SUCCESS, string(inet_ntoa(sender_addr.sin_addr)) + " : "
			       	+ packetTypeToString(pkt.getType()) + " : " + to_string(recv_byte) + " Bytes");

		// check the packet and take action according its type 
		processPacket(pkt, sender_addr);

		// free packet buffer data;
		pkt.freePacketData();
	}
    	close(soc);
}

// create socket bind an available port save that port and listen on that socket
int Network :: setTcpListenPort(){
	int soc = -1;
	if((soc = listenTcp(bindAndSetTcpPort(getTcpSocket()))) < 0){ 
		STOP(ERROR, "socket not set properly"); 
	}

	return soc;
}

/*
int Network :: bindTcp(){
	int soc = -1;
	if((soc =  getTcpSocket()) < 0){ STOP(ERROR, ""); }

	set<int> ports = getInfo<set<int>>(PORTS);

	if(ports.empty()){ STOP(ERROR, ""); }

	set<int>::iterator it = ports.begin();
	while(it != ports.end()){
		sockaddr_in add;
		memset(&add, 0, sizeof(sockaddr_in));
		add.sin_family = AF_INET;
		add.sin_port = htons(*it);
		add.sin_addr.s_addr = INADDR_ANY;

		if(bind(soc, (sockaddr*) &add, sizeof(add)) < 0) { SYSLOG; it++; }
		else {	Sys :: log("port configure " + *it); break; }
	}
	if(it == ports.end()){ SYSLOG; SYSLOG(INFO, "Running Out Of Port"); STOP; }
	else { return soc; }
}
*/

/*
int Network :: listenTcp(){

	int soc = -1;
	if((soc = bindTcp()) < 0){ STOP(ERROR, ""); }

	if(listen(soc, getInfo<int>(MCB)) < 0) { SYSLOG; }

	return soc;
}
*/
// accept connections on tcp socket and returns client socket
/*
int Network :: acceptTcp(int soc){
	if(soc < 0) return soc;

	int csoc = -1;
	sockaddr_in c_addr;
	socklen_t c_addr_len = sizeof(c_addr);

	if((csoc = accept(soc, (struct sockaddr*) &c_addr, &c_addr_len)) < 0){ SYSLOG; }
	else { SYSLOG(INFO, "new connection found"); Conf :: a_book.add(c_addr.sin_addr, c_addr.sin_port); }
	
	return csoc;
}
*/
/*
int Network :: connectTcp(in_addr ip){
	int soc = -1;
	if((soc = bindTcp()) < 0){ STOP(ERROR, ""); }

	set<int> ports = getInfo<set<int>>(PORTS);

	if(ports.empty()){ STOP(ERROR, ""); }

	set<int>::iterator it = ports.begin();
	while(it != ports.end()){
		if(connectTcp(soc, ip, *it) < 0){ SYSLOG; ++it; }
		else{ break; }
	}

	return soc;
}
int Network :: connectTcp(int soc, in_addr ip, unsigned short int port){
	if(soc < 0) return soc;

	sockaddr_in s_addr;
	socklen_t s_addr_len = sizeof(s_addr);

	memset(&s_addr, 0, sizeof(sockaddr_in));
	s_addr.sin_family = AF_INET;
	s_addr.sin_port = htons(port);
	s_addr.sin_addr = ip;

	if(connect(soc,(sockaddr*) &s_addr, s_addr_len) < 0) { SYSLOG;}

	return soc;
}
*/
