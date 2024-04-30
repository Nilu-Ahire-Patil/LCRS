// #include "conf.h"
#include <cstring> 	//memset
#include <arpa/inet.h>
#include <iostream>

using namespace std;

struct addr_list {
	struct in_addr addr = { 0 };
	unsigned short int port = { 0 };
};

struct addr_book {
	addr_list* a_list;
	int* clientSockets;
	int size;
};

class AddrList {
	public:
		void add(struct addr_book*, struct in_addr, unsigned short int, int);
		void print(addr_book*);
};

void AddrList :: add(struct addr_book* a_book, struct in_addr c_addr, unsigned short int port, int soc){
	if(a_book == NULL){ exit(1);}

	if(a_book->size >= Conf :: getInfo<int>(MAC)){ a_book->size = 0;}

	a_book->a_list[a_book->size].addr = c_addr;
	a_book->a_list[a_book->size].port = port;
	a_book->clientSockets[a_book->size++] = soc;
	//a_book->size++;
}

void AddrList :: print(addr_book* a_book){
	if(a_book == NULL){ exit(1);}

	int loop = 0;
	while(loop < a_book->size){
		char CON_IP_ADD[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &(a_book->a_list[loop].addr), CON_IP_ADD, INET_ADDRSTRLEN);
		printf("[%d]\t[%s : %d]\n",loop,CON_IP_ADD,a_book->a_list[loop].port);
		loop++;
	}
}
