#pragma once

#include <arpa/inet.h>
#include <ctime>
#include <cerrno>
#include <cstring>
#include <iostream>

//auto generated system log for error and warnings 
#define ERROR 	"\033[31mError\033[0m"
#define INFO 	"\033[32mInfo\033[0m"
#define WARN 	"\033[33mWarning\033[0m"
#define SUCCESS "\033[34mSuccess\033[0m"

#define SYSLOG(logType, msg) Sys::log(__FILE__, __LINE__, logType, msg)
#define STOP(logType, msg) SYSLOG(logType, msg); exit(EXIT_FAILURE)

class Sys {
	public:
		//system log with file name and line number
		//prints system error
		static void log(const char*, int, const char*, const std::string&);

};

/*-------------------------------------------------------------------------------------------------*/

void Sys::log(const char* fileName, int lineNo, const char* logType, const std::string& msg) {
	//Current time stamp
	time_t now = std::time(NULL);
	char timestamp[64];
	strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
	std::string line = "[" + std::string(timestamp) + "]";
       	line += "[" + std::string(logType) + "]";
      	line += ":" + std::string(fileName);
       	line += ":" + std::to_string(lineNo);
	line += ":" + std::string(msg);
	if(errno){ 
		line += ":" + std::string(strerror(errno)); 
		errno = 0;
	}

	fprintf(stderr, "%s\n", line.c_str());
}

/*-------------------------------------------------------------------------------------------------*/

struct n_addr {
	in_addr ip = { 0 };
	unsigned short port = { 0 };

	bool operator<(const n_addr& other) const {
		return ip.s_addr < other.ip.s_addr || (ip.s_addr == other.ip.s_addr && port < other.port);
	}

	n_addr(in_addr ip, unsigned short port) : ip(ip), port(port) {}
};

/*-------------------------------------------------------------------------------------------------*/

enum class packetType { 
	Unknown = 0x00,
	UdpHandshake = 0x01,
};

struct packetHeader {
	packetType type;
	unsigned int length = { 0 };
};

struct packet {
	packetHeader header;	//contains meta data of data stored in packet
	char* data;		//contains data 

	//return size of data stored in packet
	unsigned int getSize(){ return this->header.length; }

	//return complete size of packet including header
	unsigned int getFullSize(){ return sizeof(packetHeader) + this->getSize(); }

	//returns packet type
	packetType getType(){ return header.type; }

	// converts packet to continuous memory block of characters
	char* serialize(){
		//reserve continues memory for store packet data
		char* buffer = new char[sizeof(packetHeader) + this->getSize()]{ '\0' };
		//copy header data first
		memcpy(buffer, &this->header, sizeof(packetHeader));
		//copy packet data into buffer
		memcpy(buffer + sizeof(packetHeader), this->data, this->getSize());
		//return base address of continues data stored memory place
		return buffer;
	}

	// converts continuous memory block of character to packet
	void deserialize(const char* buffer) {
		//copy header data
		memcpy(&this->header, buffer, sizeof(packetHeader));
		//reserve memory for store packet data
    		this->data = new char[this->getSize()]{ '\0' };
		//copy structure data from buffer
    		memcpy(this->data, buffer + sizeof(packetHeader), this->getSize());
	}

	// initialise packet
	void init(packetType type, void* data, unsigned int size){
		this->header.type = type;
		this->header.length = size;
		this->data = new char[size]{ '\0' };
		memcpy(this->data, data, size);
	}
	
	// removes reserv memory used by packet data
	void freePacketData(){ delete[] this->data; this->data = nullptr; }

};

// usefull for converting packetType to string
std::string packetTypeToString(packetType type) {
    switch (type) {
        case packetType::Unknown: return "Unknown";
        case packetType::UdpHandshake: return "UdpHandshake";
        default: return "..?";
    }
}

/*-------------------------------------------------------------------------------------------------*/

struct addr_book {
	std::set<n_addr> addr_set; //address list
	
	//add n_addr and port in addr_set;	
	void add(n_addr& oth){ 
		addr_set.insert(oth);  
		SYSLOG(INFO, std::string(inet_ntoa(oth.ip)) + ":" + std::to_string(oth.port) + ":address save");
	}

	//add in_addr and port in addr_set
	void add(in_addr& ip, unsigned short port){
		n_addr addr(ip, port);
		add(addr);
	}

	//add string and port in addr_set
	void add(const std::string& ip, unsigned short port){
		in_addr addr;
		inet_pton(AF_INET, ip.c_str(), &addr);
		add(addr, port);
	}

	//return length of addr_book;
	int size(){ return addr_set.size(); }
};

/*-------------------------------------------------------------------------------------------------*/

/*
void Sys :: log(const string& msg) {
	time_t now = time(NULL);
	char timestamp[64];
	strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", localtime(&now));
 	fprintf(stderr, "[%s] [Info] :%s\n", timestamp, msg.c_str());
}
*/
