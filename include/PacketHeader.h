#ifndef PACKET_HEADER_H
#define PACKET_HEADER_H

#include "SysId.h"		// sys_id

#include <string>		// string

/*-------------------------------------------------------------------------------------------------*/

enum class packetType { 
	Unknown = 0x00,
	UdpHandshake = 0x01,
	Message = 0x02
};

/*-------------------------------------------------------------------------------------------------*/

struct packetHeader {
	private:
		packetType _type;		// packet type
		sys_id _s_id;			// sender system id
		unsigned int _dataSize = { 0 };	// size of packet data

		// default constructor
		packetHeader();
	
		// initialise header using char*
		packetHeader(const char*);

		// friend structure to access private variables
		friend struct packet;

	public:
		// initialise packet header
		packetHeader(packetType, unsigned int);

		// return sender system id
		const sys_id& s_id() const;

		// returns packet type
		packetType type() const;

		// returns packet data size	
		unsigned int dataSize() const;
};

/*-------------------------------------------------------------------------------------------------*/
/*
#ifndef PACKET_HEADER_IMPL_H
#include "../src/PacketHeader.cpp"
#endif
*/
#endif

/*-------------------------------------------------------------------------------------------------*/
