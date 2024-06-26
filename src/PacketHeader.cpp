#ifndef PACKET_HEADER_IMPL_H
#define PACKET_HEADER_IMPL_H

#include "PacketHeader.h"	// interface
#include "SysId.h"		// sys_id
#include "Configure.h"		// getSysId

#include <string>		// string
#include <cstring>		// memcpy

/*-------------------------------------------------------------------------------------------------*/

// default constructor
packetHeader::packetHeader(){}

// initialise packet header using char*
packetHeader::packetHeader(const char* buffer): _s_id(buffer) {
	// set type of packet
	std::memcpy(&_type, buffer + sizeof(sys_id), sizeof(packetType));

	// set data size
	std::memcpy(&_dataSize, buffer + sizeof(sys_id) + sizeof(packetType), sizeof(unsigned int));
}

// initialise packet header
packetHeader::packetHeader(packetType type, unsigned int dataSize): _type(type), _dataSize(dataSize){
	// copy system id
	_s_id = sys_id(Conf::getSysId());
}

// return sender system id
const sys_id& packetHeader::s_id() const { return this->_s_id; }

// returns packet type
packetType packetHeader::type() const { return this->_type; }

// returns data length in bytes
unsigned int packetHeader::dataSize() const { return this->_dataSize; }

/*-------------------------------------------------------------------------------------------------*/

// use full for converting packet type to string
std::string packetTypeToString(packetType type){
    switch (type) {
        case packetType::REQ_HANDSHAKE: return "REQ_HANDSHAKE";
        case packetType::RES_HANDSHAKE: return "RES_HANDSHAKE";
        case packetType::REQ_ADOPTER: return "REQ_ADOPTER";
        case packetType::RES_ADOPTER: return "RES_ADOPTER";
	case packetType::REQ_STORE: return "REQ_STORE";
	case packetType::SUC_STORE: return "SUC_STORE";
	case packetType::ERR_STORE: return "ERR_STORE";
	case packetType::REQ_RETRIEVE: return "REQ_RETRIEVE";
	case packetType::RES_RETRIEVE: return "RES_RETRIEVE";
	case packetType::REQ_UNKNOWN: return "REQ_UNKNOWN";
        case packetType::TXT_MESSAGE: return "TXT_MESSAGE";
        default: return "..?";
    }
}

/*-------------------------------------------------------------------------------------------------*/
#endif
