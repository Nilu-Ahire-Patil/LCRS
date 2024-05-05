#pragma once

#include "network.h"

//#include "conf.h"
//#include "mConfig.h"
//#include "addrList.h"

using namespace std;

class Server {

	addr_book* a_book = new addr_book;

	int acceptClient(int);

	public:
		void start(); 					//ONLY FOR SERVER
		addr_book* getAddrBook(int, in_addr, unsigned short); 	//ONLY FOR CLIENT
									//
	Server(){
		Conf :: initConf("init.conf");
		this->a_book->a_list = new addr_list[Conf :: getInfo<int>(MAC)] { 0 };
		this->a_book->clientSockets = new int[Conf :: getInfo<int>(MAC)] { -1 };
	}
};

void Server :: start(){

	Network nt;
	int soc = -1;
	if((soc = nt.listenTcp(nt.bindTcp(nt.getTcpSocket()))) < 0){ SYSLOG; exit(1);}
	while(1){ 
		int csoc = -1;
		if((csoc = acceptClient(soc)) < 0){ SYSLOG; continue;}
	}
}

int Server :: acceptClient(int soc){
	if(soc < 0) return soc;

	sockaddr_in c_addr; 
	socklen_t c_addr_len = sizeof(c_addr);

	int csoc = -1;
	if((csoc = accept(soc, (sockaddr*) &c_addr, &c_addr_len)) < 0) { SYSLOG; return -1;}

	//get client port
	unsigned short listning_port = -1;
	int recvByte = 0;
	if((recvByte = recv(csoc, &listning_port, sizeof(unsigned short), 0)) < 0) { SYSLOG; return -1;}
	else{ Sys :: log("port received	" + recvByte); }

	//send size of addr_book
	int sendBytes = -1;
	if((sendBytes = send(csoc, &this->a_book->size, sizeof(int), 0)) < 0){ SYSLOG;}
	else{ Sys :: log("addr_book size sent " + sendBytes); }

	///send addr_list
	sendBytes = -1;
	if((sendBytes = send(csoc, this->a_book->a_list, sizeof(addr_list) * this->a_book->size, 0)) < 0){ SYSLOG;}
	else{ Sys :: log("addr_list sent" + sendBytes); }

	AddrList AL;
	AL.add(this->a_book, c_addr.sin_addr, listning_port, csoc); 

	Sys :: log("new client connected");

	//-----------
	char cli_ip_buff[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &(c_addr.sin_addr), cli_ip_buff, INET_ADDRSTRLEN);
	printf("[CONNECT]\t[%s : %d]\n", cli_ip_buff, c_addr.sin_port);
	//-----------

	return csoc;
}	

addr_book* Server :: getAddrBook(int soc, in_addr ip, unsigned short listning_port){
	if(soc < 0) return NULL;

	Network nt;
	int csoc = -1;
	if((csoc = nt.connectTcp(soc, ip)) < 0) { SYSLOG; return NULL;}

	//send listening port
	int sendBytes = -1;
	if((sendBytes = send(csoc, &listning_port, sizeof(unsigned short), 0)) < 0){ SYSLOG; return NULL;}
	else { Sys :: log("port sent. "); }

	//get size of addr_book first
	int recvByte = -1;
	if((recvByte = recv(csoc, &this->a_book->size, sizeof(int), 0)) < 0) { SYSLOG; return NULL;}
	else{ Sys :: log("received addr_book size" + recvByte); }

	//addr_list* a_list = new addr_list;
	//get addr_list
	recvByte = -1;
	if((recvByte = recv(csoc, this->a_book->a_list, sizeof(addr_list) * this->a_book->size, 0)) < 0) { SYSLOG; return NULL;}
	else if (recvByte != sizeof(addr_list) * this->a_book->size){ Sys :: log("Incomplete addr_list receive"); }
	else { Sys :: log("addr_list receive " + recvByte); }

	return this->a_book;
}
