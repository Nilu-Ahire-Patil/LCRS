#ifndef N_ADDR_IMPL_H
#define N_ADDR_IMPL_H

#include "N_addr.h"		// interface

#include <string>		// string

/*-------------------------------------------------------------------------------------------------*/

// default constructor
n_addr::n_addr(){}

// crate n_addr object with ip and port pair
n_addr::n_addr(in_addr ip, unsigned short port): _ip(ip), _port(port) {}

// crate n_addr object with string ip and port pair
n_addr::n_addr(std::string ip, unsigned short port): _port(port) {
	this->_ip.s_addr = inet_addr(ip.c_str());
}

// for compare two objects of n_addr
bool n_addr::operator<(const n_addr& other) const {
	return (
			(this->_ip.s_addr < other._ip.s_addr) || 
			(this->_ip.s_addr == other._ip.s_addr && this->_port < other._port)
	       );
}

// return ip address
const in_addr& n_addr::ip() const { return this->_ip; }

// return ip address
std::string n_addr::str_ip() const {
	char buffer[INET_ADDRSTRLEN]{ 0 };

    	// Convert in_addr to string
    	if(inet_ntop(AF_INET, &_ip, buffer, INET_ADDRSTRLEN) == nullptr){ return ""; }
        return std::string(buffer);
}

// return port
unsigned short n_addr::port() const { return this->_port; }

/*-------------------------------------------------------------------------------------------------*/
#endif
