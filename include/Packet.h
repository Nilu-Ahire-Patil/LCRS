#ifndef PACKET_H
#define PACKET_H

#include "PacketHeader.h"	// packetHeader
				// packetType
#include "SysId.h"		// sys_id

#include <string>		// string

/*-------------------------------------------------------------------------------------------------*/

struct packet {
	private:
		packetHeader _header;		//contains packet meta data
		char* _data;			//contains packet data 

		// default constructor
		packet();

	public:
		// convert continuous memory block of character to packet
		// worrk as deserialiser
		packet(const char* buffer);

		// initialise packet
		packet(packetType, void*, unsigned int);

		// default constructor
		~packet();

		// convert packet to continuous memory block of characters
		char* serialize() const;

		// return complete size of packet including header
		unsigned int size() const;

		// return complete size of packet including header
		unsigned int headerSize() const;

		// return size of data stored in packet
		unsigned int dataSize() const;

		// sender system id
		const sys_id& s_id() const;

		// return packet type
		packetType type() const;

		// return packet data
		char* data() const;

		// return header
	//	const packetHeader* header() const;

};

/*-------------------------------------------------------------------------------------------------*/

// use for converting packet type to string
std::string packetTypeToString(packetType);

/*-------------------------------------------------------------------------------------------------*/
/*
#ifndef PACKET_IMPL_H
#include "../src/Packet.cpp"
#endif
*/
#endif

/*-------------------------------------------------------------------------------------------------*/
