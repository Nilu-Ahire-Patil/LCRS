#ifndef ADDRBOOK_H
#define ADDRBOOK_H

#include <arpa/inet.h>		// in_addr
#include <string> 		// string
#include <set>			// set

/*-------------------------------------------------------------------------------------------------*/

struct n_addr {
	private:
		in_addr _ip = { 0 };				// node ip address
		unsigned short _port = { 0 };			// node port

	public:
		bool operator<(const n_addr&) const;  	// compare two nodes

		n_addr(in_addr, unsigned short);	// create new node with ip and port

		n_addr(std::string, unsigned short); 	// create new node with ip string and port

		in_addr ip() const;			// return ip address

		unsigned short port() const;		// return port

};

/*-------------------------------------------------------------------------------------------------*/

struct addr_book {
	public:
		std::set<n_addr> addr_set; 		// node address list
	
		//add n_addr and port in addr_set;	
		void insert(n_addr&);

		//add in_addr and port in addr_set
		void insert(in_addr&, unsigned short);
	
		//add string and port in addr_set
		void insert(const std::string&, unsigned short);

		//return length of addr_book;
		int size() const;
};

/*-------------------------------------------------------------------------------------------------*/
/*
#ifndef ADDRBOOK_IMPL_H
#include "../src/AddrBook.cpp"
#endif
*/
#endif

/*-------------------------------------------------------------------------------------------------*/
