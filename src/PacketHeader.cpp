#ifndef PACKET_HEADER_IMPL_H
#define PACKET_HEEADER_IMPL_H

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
	memcpy(&this->_s_id._id, Conf::getSysId(), sizeof(sys_id));
}

// return sender system id
const sys_id& packetHeader::s_id() const { return this->_s_id; }

// returns packet type
packetType packetHeader::type() const { return this->_type; }

// returns data length in bytes
unsigned int packetHeader::dataSize() const { return this->_dataSize; }

/*-------------------------------------------------------------------------------------------------*/
#endif