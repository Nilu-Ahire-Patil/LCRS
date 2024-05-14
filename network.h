#pragma once

//#include "addrList.h"

//#include <arpa/inet.h>
//#include <set>
//#include <iostream>
//#include <cstring>
#include <unistd.h>
#include "conf.h"

using namespace std;

class Network : public Conf {

		int getUdpSocket();
		int bindUdp();
		int broadcast(packet&);
		int joinSingleMulticastGroup();
		void receveAndReplyBroadcast();

		int getTcpSocket();
	//	int bindTcp();

	public:

		int updateAddrSet(unsigned short);

		int bindTcp(unsigned short*);

	//	int listenTcp();
		int listenTcp(int);

		int acceptTcp(int);

	//	int connectTcp(int, in_addr);
	//	int connectTcp(int, in_addr, unsigned short);
};

int Network :: getTcpSocket(){

	int soc = -1;
	if((soc = socket(AF_INET, SOCK_STREAM, 0)) < 0){ SYSLOG; STOP; }

	int optval = 1;
	if(setsockopt(soc, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0){ SYSLOG; }

	return soc;
}

int Network :: getUdpSocket(){

	int soc = -1;
	if((soc = socket(AF_INET, SOCK_DGRAM, 0)) < 0){ SYSLOG; STOP; }

	int broadcast_flag = 1;
	if(setsockopt(soc, SOL_SOCKET, SO_BROADCAST, (char*)&broadcast_flag, sizeof(broadcast_flag)) < 0){ SYSLOG; STOP; }

	int TTL = 1;
	if(setsockopt(soc, IPPROTO_IP,IP_MULTICAST_TTL, (char*)&TTL , sizeof(TTL)) < 0){ SYSLOG; }

	return soc;
}

int Network :: broadcast(packet& pkt){
	int soc = -1;
	if((soc = getUdpSocket()) < 0){ SYSLOG; STOP; }

	char* buffer = pkt.serialize();

	struct sockaddr_in oth_addr;
	memset(&oth_addr, 0, sizeof(struct sockaddr_in));
    	oth_addr.sin_family = AF_INET;

	set<string> bga = getInfo<set<string>>(BGA);

	if(bga.empty()){ SYSLOG; STOP; }

	set<string>::iterator it = bga.begin(); 
	while(it != bga.end()){

		oth_addr.sin_addr.s_addr = inet_addr((*it).c_str()); //broadcast Group Address

		set<int> ports = getInfo<set<int>>(UDP_PORTS);

		if(ports.empty()){ SYSLOG; STOP; }

		set<int>::iterator pit = ports.begin(); 
		while(pit != ports.end()){

    			oth_addr.sin_port = htons(*pit);

			int s_byte;
    			if((s_byte = sendto(soc, buffer, pkt.getFullSize(), 0, (struct sockaddr*)&oth_addr, sizeof(struct sockaddr_in))) < 0){ SYSLOG; }
			++pit;
		}
		++it;
    	}

	USRLOG("broadcast sent to all");
	close(soc);
	return 0;
}

int Network :: joinSingleMulticastGroup(){
	int soc = -1;
	if((soc = bindUdp()) < 0){ SYSLOG; STOP; }

	ip_mreq mreq;
	mreq.imr_interface.s_addr = htonl(INADDR_ANY);

	set<string> bga = getInfo<set<string>>(BGA);
	if(bga.empty()){ SYSLOG; STOP; }
	set<string>::iterator it = bga.begin(); 

	while(it != bga.end()){
		mreq.imr_multiaddr.s_addr = inet_addr((*it).c_str()); // Multicast group address
	
		if(setsockopt(soc, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&mreq, sizeof(mreq)) < 0){ SYSLOG; }
		else { USRLOG(*it + " : multicast group join"); return soc; } 
		it++;
	}
	if(it == bga.end()){ SYSLOG; STOP; }

	return -1;
}

void Network :: receveAndReplyBroadcast(){
	int soc = -1;
	if((soc = joinSingleMulticastGroup()) < 0){ SYSLOG; STOP; }

    	sockaddr_in oth_addr = { 0 };
	socklen_t oth_addr_len = sizeof(oth_addr);

	while(1){
		char buffer[MAX_PACKET_SIZE] = { '\0' };
		int recv_byte;

    		if((recv_byte = recvfrom(soc, buffer, MAX_PACKET_SIZE, 0, (sockaddr*) &oth_addr, &oth_addr_len)) < 0){ SYSLOG; continue; }
		packet pkt;
		pkt.deserialize(buffer);
		USRLOG(string(inet_ntoa(oth_addr.sin_addr)) + " : " + packetTypeToString(pkt.getType()) + " : " + to_string(recv_byte) + " Bytes");
/*		printf("[RECEIVE] [%s] [%d byte]\n", inet_ntoa(oth_addr.sin_addr), recv_byte);
		
		cout << "type : " << packetTypeToString(pkt.getType()) << endl;
		cout << "length : " << pkt.getSize() << endl;
		cout << "full size : " << pkt.getFullSize() << endl;
		cout << "data : " << *(unsigned short*)pkt.data << endl;
*/
		pkt.freePacketData();
	}
    	close(soc);
}


int Network :: updateAddrSet(unsigned short listening_port){
	//creating packet of listening port witih port flag
	packet pkt;
	//initialise paket;
	pkt.init(packetType :: UdpHandshake, &listening_port, sizeof(unsigned short));
	//send listening port in broadcast
	if(broadcast(pkt) < 0){ SYSLOG; STOP; }
	//free reserv memory used for serializing the packet
	pkt.freePacketData();
	//accepting broadcast
	receveAndReplyBroadcast();
		
	//replying broadcast
	return -1;
}

int Network :: bindUdp(){
	int soc = -1;
	if((soc =  getUdpSocket()) < 0){ SYSLOG; STOP; }
 
	set<int> ports = getInfo<set<int>>(UDP_PORTS);

	if(ports.empty()){ SYSLOG; STOP; }

	set<int>::iterator it = ports.begin();
	while(it != ports.end()){
		sockaddr_in add;
		memset(&add, 0, sizeof(sockaddr_in));
		add.sin_family = AF_INET;
		add.sin_port = htons(*it);
		add.sin_addr.s_addr = INADDR_ANY;

		if(bind(soc, (sockaddr*) &add, sizeof(sockaddr_in)) < 0){ SYSLOG; it++; }
		else { USRLOG("UDP port configure to : " + to_string(*it)); break; }
	}
	if(it == ports.end()){ SYSLOG; USRLOG("Running Out Of UDP Port"); STOP; }
	else { return soc; }
}

int Network :: bindTcp(unsigned short* listening_port){
	int soc = -1;
	if((soc =  getTcpSocket()) < 0){ SYSLOG; STOP; }
 
	set<int> ports = getInfo<set<int>>(TCP_PORTS);

	if(ports.empty()){ SYSLOG; STOP; }

	set<int>::iterator it = ports.begin();
	while(it != ports.end()){
		sockaddr_in add;
		memset(&add, 0, sizeof(sockaddr_in));
		add.sin_family = AF_INET;
		add.sin_port = htons(*it);
		add.sin_addr.s_addr = INADDR_ANY;

		if(bind(soc, (sockaddr*) &add, sizeof(sockaddr_in)) < 0) { SYSLOG; it++; }
		else {
			*listening_port = *it;
			USRLOG("tcp port configure to : " + to_string(*it));
			break;
		}
	}
	if(it == ports.end()){ SYSLOG; USRLOG("Running Out Of TCP Port"); STOP; }
	else { return soc; }
}

/*
int Network :: bindTcp(){
	int soc = -1;
	if((soc =  getTcpSocket()) < 0){ SYSLOG; STOP; }

	set<int> ports = getInfo<set<int>>(PORTS);

	if(ports.empty()){ SYSLOG; STOP; }

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
	if(it == ports.end()){ SYSLOG; USRLOG("Running Out Of Port"); STOP; }
	else { return soc; }
}
*/
int Network :: listenTcp(int soc){

	if(soc < 0){ SYSLOG; STOP; }

	if(listen(soc, getInfo<int>(MCB)) < 0) { SYSLOG; STOP; }

	return soc;
}
/*
int Network :: listenTcp(){

	int soc = -1;
	if((soc = bindTcp()) < 0){ SYSLOG; STOP; }

	if(listen(soc, getInfo<int>(MCB)) < 0) { SYSLOG; }

	return soc;
}
*/
int Network :: acceptTcp(int soc){
	if(soc < 0) return soc;

	int csoc = -1;
	sockaddr_in c_addr;
	socklen_t c_addr_len = sizeof(c_addr);

	if((csoc = accept(soc, (struct sockaddr*) &c_addr, &c_addr_len)) < 0) { SYSLOG; }

	return csoc;
}
/*
int Network :: connectTcp(in_addr ip){
	int soc = -1;
	if((soc = bindTcp()) < 0){ SYSLOG; STOP; }

	set<int> ports = getInfo<set<int>>(PORTS);

	if(ports.empty()){ SYSLOG; STOP; }

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
