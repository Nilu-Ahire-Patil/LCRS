#ifndef PACKET_H
#define PACKET_H

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
		packetType _type;			// packet type
		unsigned int _dataSize = { 0 };		// size of packet data

		// set packet type
		void type(packetType);

		// set packet data size	
		void dataSize(unsigned int);

		// default constructor
		packetHeader();

		// friend structure to access private variables
		friend struct packet;

	public:
		// returns packet type
		packetType type() const;

		// returns packet data size	
		unsigned int dataSize() const;

		// initialise packet header
		packetHeader(packetType, unsigned int);
};

/*-------------------------------------------------------------------------------------------------*/

struct packet {
	private:
		packetHeader _header;	//contains packet meta data
		char* _data;		//contains packet data 

		// set packet header
		void header(packetHeader);

		// set packet data
		void data(char*);

	public:

		// return complete size of packet including header
		unsigned int size() const;

		// return size of data stored in packet
		unsigned int dataSize() const;

		// return complete size of packet including header
		unsigned int headerSize() const;

		// return packet type
		packetType type() const;

		// return packet data
		char* data() const;

		// return header
		const packetHeader* header() const;

		// convert packet to continuous memory block of characters
		char* serialize() const;

		// convert continuous memory block of character to packet
		void deserialize(const char* buffer);

		// remove reserve memory used by packet data
		void freePacketData();

		// initialise packet
		packet(packetType, void*, unsigned int);

		// default constructor
		packet();
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
