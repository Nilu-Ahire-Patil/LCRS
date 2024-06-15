#ifndef PROTOCOL_IMPL_H
#define PROTOCOL_IMPL_H

#include "Protocol.h"		// interface

#include "Log.h"		// SYSLOG
#include "Configure.h"		// Conf
#include "Network.h"		// Network
#include "MetaInfo.h"		// chunk
#include "Storage.h"		// storeChunk, storeMeta

#include <string>	
#include <cstring>
/*-------------------------------------------------------------------------------------------------*/

// adopter book for store file chunks
std::unordered_map<sys_id, n_addr, sys_id_hash> Protocol::adopter_book;

// mux for solve critical section problem
std::shared_mutex Protocol::adopter_bookMutex;

/*-------------------------------------------------------------------------------------------------*/

// initialize tcp base packet reply
int Protocol::reqUdpHandshake(){

	// broadcast our listening port to all connected groups and all available ports 
	unsigned short listening_port = 0;
	if((listening_port = Conf::getInfo<unsigned short>(LP)) == 0){ 
		SYSLOG(ERROR, "BAD_PORT"); 
		return -1;
	}

	// creating packet of listening port with port flag
	packet pkt(packetType::REQ_HANDSHAKE, &listening_port, sizeof(unsigned short));

	//send listening port in broadcast
	Network nt;
	return nt.broadcast(pkt);
}

int Protocol::repTcpHandshake(const n_addr addr){
	// broadcast our listening port to all connected groups and all available ports 
	unsigned short listening_port = 0;
	if((listening_port = Conf::getInfo<unsigned short>(LP)) == 0){ 
		SYSLOG(ERROR, "BAD_PORT"); 
		return -1;
	}

	// creating packet of listening port with port flag
	packet pkt(packetType::RES_HANDSHAKE, &listening_port, sizeof(unsigned short));

	// send listening port packet using tcp
	Network nt;
	return nt.sendTcpPacket(pkt, addr);
}

// request adopter nodes for store chunk of files
int Protocol::reqUdpAdopter(){ 

	// reply with our listening port  
	unsigned short listening_port = 0;
	if((listening_port = Conf::getInfo<unsigned short>(LP)) == 0){
		SYSLOG(ERROR, "BAD_PORT"); 
		return -1;
	}

	// creating packet of listening port with port flag
	packet pkt(packetType::REQ_ADOPTER, &listening_port, sizeof(unsigned short));

	//send listening port in broadcast
	Network nt;
	return nt.broadcast(pkt);
}

// reply for request adopter nodes for store chunk of files
int Protocol::repTcpAdopter(const n_addr addr){ 

	// check in our memory the sppace is empty or not
	if(Conf::getInfo<int>(AVL_MEM) <= 0){ 
		SYSLOG(ERROR, "NO_SPACE_AVAILABLE_LOCALY"); 
		return -1;
	}

	// if we have empty space then reply 
	// other wise no need to reply

	// reply with our listening port  
	// we can also add more information
	// like remaining space
	// maximum capacity and other
	unsigned short listening_port = 0;
	if((listening_port = Conf::getInfo<unsigned short>(LP)) == 0){ 
		SYSLOG(ERROR, "BAD_PORT"); 
		return -1;
       	}

	// creating packet of listening port with port flag
	packet pkt(packetType::RES_ADOPTER, &listening_port, sizeof(unsigned short));

	// send listening port packet using tcp
	Network nt;
	return nt.sendTcpPacket(pkt, addr);
}

// send chunk to address using tcp
int Protocol::reqTcpStore(const n_addr addr, const chunk& chnk){

	char* buffer = chnk.serialize();

	// creating packet of listening port with port flag
	packet pkt(packetType::REQ_STORE, buffer, chnk.dataSize());

	// free chunk serialize
	delete[] buffer;

	// send listening port packet using tcp
	Network nt;
	return nt.sendTcpPacket(pkt, addr);
}

// accept or reject chunk sttore and reply if stored 
int Protocol::repTcpStore(const n_addr addr, const chunk& chnk){
	// check in our memory the sppace is empty or not
	int memory = 0;
	if((memory = Conf::getInfo<int>(AVL_MEM)) <= 0){ 
		SYSLOG(ERROR, "NO_SPACE_AVAILABLE_LOCALY"); 
		return -1;
	}

	// store chunk in memory
	Storage st;
	if(st.storeChunk(chnk) < 0){
		SYSLOG(ERROR, "STORE_CHUNK_FAIL"); 
		return -1;
	}

	// send chunk id return in case of success
	char* buffer = chnk.serialize();

	// creating packet of chunk id
	packet pkts(packetType::SUC_STORE, buffer, sizeof(chunkHeader));

	// send chunk id return in case of success
	Network nt;
	if(nt.sendTcpPacket(pkts, addr) < 0){

		// creating packet of chunk id with ERR_STORE FLAG
		packet pkte(packetType::ERR_STORE, buffer, sizeof(chunkHeader));

		// send chunk id with ERR_STORE request
		return nt.sendTcpPacket(pkte, addr);
	}	
	return 0;
}

// send message using tcp
int Protocol::repTcpMessage(const n_addr& addr, const std::string& msg){

	// create message packet
	packet pkt(packetType::TXT_MESSAGE,(void*) msg.c_str(), msg.length());

	// send packet using tcp
	Network nt;
	return nt.sendTcpPacket(pkt, addr);
}


/*-------------------------------------------------------------------------------------------------*/
#endif

