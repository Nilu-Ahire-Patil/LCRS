#include <cstring> 	//memset
#include <arpa/inet.h>
#include <iostream>

using namespace std;

struct addr_list {
	struct in_addr list[Conf :: max_add_capacity] =  { 0 };
	unsigned short int port[Conf :: max_add_capacity] = { 0 };
	unsigned int size = 0;
};

class AddrList {
	public:
		void add(struct addr_list*, struct in_addr, unsigned short int);
		void print(addr_list*);
};

void AddrList :: add(struct addr_list* a_list, struct in_addr c_addr, unsigned short int port){
	if(a_list == NULL)	exit(1);

	if(a_list->size >= Conf :: max_add_capacity)	a_list->size = 0;
	a_list->list[a_list->size] = c_addr;
	a_list->port[a_list->size] = port;
	a_list->size++;
}

void AddrList :: print(addr_list* a_list){
	if(a_list == NULL)	exit(1);

	int loop = 0;
	while(loop < a_list->size){
		char CON_IP_ADD[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &(a_list->list[loop]), CON_IP_ADD, INET_ADDRSTRLEN);
		printf("[%d]\t[%s : %d]\n",loop,CON_IP_ADD,a_list->port[loop]);
		loop++;
	}
}
