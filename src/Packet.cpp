#ifndef PACKET_IMPL_H
#define PACKET_IMPL_H

#include "Packet.h"		// interface
#include "PacketHeader.h"	// packetHeader
#include "SysId.h"		// sys_id

#include <arpa/inet.h>		// in_addr
#include <string>		// string
#include <cstring>		// memcpy

/*-------------------------------------------------------------------------------------------------*/

// default constructor
packet::packet(){}

// converts continuous memory block of character to packet
packet::packet(const char* buffer){
	// copy header data
	memcpy(&this->_header, buffer, this->headerSize());

	// reserve memory for store packet data
    	this->_data = new char[this->size()]{ '\0' };

	// copy structure data from buffer
    	memcpy(this->_data, buffer + this->headerSize(), this->dataSize());
}

// initialise packet
packet::packet(packetType type, void* data, unsigned int size): _header(type, size){
	// reserve memory for packet data
	this->_data = new char[size]{ '\0' };
	memcpy(this->_data, data, size);
}

// distructor
packet::~packet(){ if(this->_data != nullptr){ delete[] this->_data; this->_data = nullptr; } }

// converts packet to continuous memory block of characters
char* packet::serialize() const {
	// reserve continues memory for store packet data
	char* buffer = new char[ sizeof(packetHeader) + this->size()]{ '\0' };

	// copy header data first
	memcpy(buffer, &this->_header, this->headerSize());

	// copy packet data into buffer
	memcpy(buffer + this->headerSize(), this->data(), this->dataSize());

	// return base address of continues data stored memory place
	return buffer;
}

// return complete size of packet including header
unsigned int packet::size() const { return this->headerSize() + this->_header.dataSize(); }

// return size of header 
unsigned int packet::headerSize() const { return sizeof(packetHeader); }

// return size of data stored in packet
unsigned int packet::dataSize() const { return this->_header.dataSize(); }

// sender system id
const sys_id& packet::s_id() const { return this->_header.s_id(); }

// sender system id
const std::string packet::str_sys_id() const { return this->_header._s_id.str_id(); }

// returns packet type
packetType packet::type() const { return this->_header.type(); }

// returns packet data
char* packet::data() const { return this->_data; }


/*-------------------------------------------------------------------------------------------------*/
#endif
