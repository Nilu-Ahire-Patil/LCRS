#ifndef PACKET_IMPL_H
#define PACKET_IMPL_H

#include "Packet.h"		// Interface

#include <arpa/inet.h>		// in_addr
#include <string>		// string
#include <cstring>		// memcpy

/*-------------------------------------------------------------------------------------------------*/

// set packet type
void packetHeader::type(packetType type){ this->_type = type; }

// set data length in bytes
void packetHeader::dataSize(unsigned int dataSize){ this->_dataSize = dataSize; }

// default constructor
packetHeader::packetHeader(){}

// returns packet type
packetType packetHeader::type() const { return this->_type; }

// returns data length in bytes
unsigned int packetHeader::dataSize() const { return this->_dataSize; }

// initialise packet header
packetHeader::packetHeader(packetType type, unsigned int dataSize): _type(type), _dataSize(dataSize){}

/*-------------------------------------------------------------------------------------------------*/

// set packet header
void packet::header(packetHeader header){ this->_header = header; }

// set packet data
void packet::data(char* _data){ this->_data = _data; }

// return complete size of packet including header
unsigned int packet::size() const { return this->headerSize() + this->_header.dataSize(); }

// return size of data stored in packet
unsigned int packet::dataSize() const { return this->_header.dataSize(); }

// return size of header 
unsigned int packet::headerSize() const { return sizeof(packetHeader); }

// returns packet type
packetType packet::type() const { return this->_header.type(); }

// returns packet data
char* packet::data() const { return this->_data; }

// return packet header
const packetHeader* packet::header() const { return &this->_header; }

// converts packet to continuous memory block of characters
char* packet::serialize() const {
	// reserve continues memory for store packet data
	char* buffer = new char[this->size()]{ '\0' };

	// copy header data first
	memcpy(buffer, &this->_header, this->headerSize());

	// copy packet data into buffer
	memcpy(buffer + this->headerSize(), this->data(), this->dataSize());

	// return base address of continues data stored memory place
	return buffer;
}

// converts continuous memory block of character to packet
void packet::deserialize(const char* buffer) {
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

// defauult constructor
packet::packet(){}
	
// removes reserve memory used by packet data
void packet::freePacketData(){ delete[] this->_data; this->data(nullptr); }

/*-------------------------------------------------------------------------------------------------*/

// use full for converting packet type to string
std::string packetTypeToString(packetType type){
    switch (type) {
        case packetType::Unknown: return "Unknown";
        case packetType::UdpHandshake: return "UdpHandshake";
        case packetType::Message: return "Message";
        default: return "..?";
    }
}

/*-------------------------------------------------------------------------------------------------*/
#endif
