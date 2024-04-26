#include "conf.h"
#include "addrList.h"
#include "network.h"


//#include<thread>		//For pthread_create ,pthread_detach, pthread_exit

//#include<sys/socket.h>	//For socket, bind, listen, accept
//#include<stdlib.h>		//For EXIT_FAILURE
//#include<string.h>		//For memset
//#include<unistd.h>		//For close
//#include<arpa/inet.h>		//For inet_addr
//#include<netinet/in.h>	//For inet_addr, INET_ADDRSTRLEN ,inet_ntop

using namespace std;

class Server {

	struct addr_list* a_list = new addr_list;
	int soc = -1;
	int acceptClient();

	public:
		void start(); 					//ONLY FOR SERVER
		struct addr_list* getAddrList(int, in_addr, unsigned short); 	//ONLY FOR CLIENT
};

void Server :: start(){

	Network nt;
	this->soc = nt.listenTcp(nt.bindTcp(nt.getTcpSocket()));
	if(this->soc < 0){ Sys :: log(__func__,__LINE__); exit(1);}
	while(1){ 
		int csoc = -1;
		if((csoc = acceptClient()) < 0) { Sys :: log(__func__,__LINE__); continue;}

		int sendBytes = 0;
		if((sendBytes = send(csoc, this->a_list, sizeof(addr_list), 0)) < 0){ Sys :: log(__func__,__LINE__);}
		else{ Sys :: log("addr_list sent"); }
	}
}

int Server :: acceptClient(){
	if(this->soc < 0) return this->soc;

	struct sockaddr_in c_addr; 
	socklen_t c_addr_len = sizeof(c_addr);
	AddrList AL;

	int csoc = -1;
	if((csoc = accept(this->soc, (struct sockaddr*) &c_addr, &c_addr_len)) < 0) { Sys :: log(__func__,__LINE__); return -1;}

	unsigned short listning_port = -1;
	int recvByte = 0;
	if((recvByte = recv(csoc, &listning_port, sizeof(unsigned short), 0)) < 0) { Sys :: log(__func__,__LINE__); return -1;}
	else{ Sys :: log("port received	"); }

	AL.add(this->a_list, c_addr.sin_addr, listning_port); 

	Sys :: log("new client connected");

	//-----------
//	char cli_ip_buff[INET_ADDRSTRLEN];
//	inet_ntop(AF_INET, &(c_addr.sin_addr), cli_ip_buff, INET_ADDRSTRLEN);
//	printf("[CONNECT]\t[%s : %d]\n", cli_ip_buff, c_addr.sin_port);
	//-----------

	return csoc;
}	

struct addr_list* Server :: getAddrList(int soc, in_addr ip, unsigned short listning_port){
	if(soc < 0) return NULL;

	Network nt;
	int csoc = -1;
	if((csoc = nt.connectTcp(soc, ip)) < 0) { Sys :: log(__func__,__LINE__); return NULL;}

	int sendBytes = -1;
	if((sendBytes = send(csoc, &listning_port, sizeof(unsigned short), 0)) < 0){ Sys :: log(__func__,__LINE__); return NULL;}
	else { Sys :: log("port sent"); }

	addr_list* a_list = new addr_list;
	int recvByte = 0;
	if((recvByte = recv(csoc, a_list, sizeof(addr_list), 0)) < 0) { Sys :: log(__func__,__LINE__); return NULL;}
	else{ Sys :: log("received addr_list"); }
	return a_list;
}
