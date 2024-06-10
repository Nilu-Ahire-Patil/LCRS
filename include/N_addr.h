#ifndef N_ADDR_H
#define N_ADDR_H

#include <arpa/inet.h>		// in_addr
#include <string> 		// string

/*-------------------------------------------------------------------------------------------------*/

struct n_addr {
	private:
		in_addr _ip{};			// node ip address
		unsigned short _port{};		// node port


	public:
		// default constructor
		n_addr();

		// create new node with ip and port
		n_addr(in_addr, unsigned short);	

		// create new node with ip string and port
	//	n_addr(std::string, unsigned short); 	

		// compare two nodes
		bool operator<(const n_addr&) const;  	

		// return ip address
		const in_addr& ip() const;			

		// return port
		unsigned short port() const;	
};

/*-------------------------------------------------------------------------------------------------*/
/*
#ifndef N_ADDR_IMPL_H
#include "../src/N_addr.cpp"
#endif
*/
#endif

/*-------------------------------------------------------------------------------------------------*/
