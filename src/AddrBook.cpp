#ifndef ADDRBOOK_IMPL_H
#define ADDRBOOK_IMPL_H

#include "AddrBook.h"		//Interface

#include <set>			// set
#include <string>		// string

/*-------------------------------------------------------------------------------------------------*/

// for compare two objects of n_addr
bool n_addr::operator<(const n_addr& other) const {
	return (
			(this->_ip.s_addr < other._ip.s_addr) || 
			(this->_ip.s_addr == other._ip.s_addr && this->_port < other._port)
	       );
}

// crate n_addr object with ip and port pair
n_addr::n_addr(in_addr ip, unsigned short port): _ip(ip), _port(port) {}

// crate n_addr object with ip and port pair
n_addr::n_addr(std::string ip, unsigned short port): _port(port) {
	this->_ip.s_addr = inet_addr(ip.c_str());
}

// return ip address
in_addr n_addr::ip() const { return this->_ip; }

// return port
unsigned short n_addr::port() const { return this->_port; }

/*-------------------------------------------------------------------------------------------------*/

// insert n_addr and port in addr_set;	
void addr_book::insert(n_addr& oth){ 
	addr_set.insert(oth);  
//	SYSLOG(INFO, std::string(inet_ntoa(oth.ip)) + ":" + std::to_string(oth.port) + ":address save");
}

// insert in_addr and port in addr_set
void addr_book::insert(in_addr& ip, unsigned short port){
	n_addr addr(ip, port);
	this->addr_set.insert(addr);  
}

// insert string and port in addr_set
void addr_book::insert(const std::string& ip, unsigned short port){
	in_addr addr;
	inet_pton(AF_INET, ip.c_str(), &addr);
	n_addr node_addr(ip, port);
	addr_set.insert(node_addr);
}

// return size of addr_book;
int addr_book::size() const { return addr_set.size(); }

/*-------------------------------------------------------------------------------------------------*/
#endif
