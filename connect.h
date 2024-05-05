//#pragma once

#include "server.h"

//#include "mConfig.h"
//#include "addrList.h"
//#include "network.h"

class Connect {
	addr_book* a_book = NULL;
	int* soc = new int[Conf :: getInfo<int>(MAC)]{ 0 };
	int* csoc = new int[Conf :: getInfo<int>(MAC)]{ 0 };
	//int cCount = 0;		//connection count
	int ssoc = -1;		//tcp listening socket
	unsigned short listenPort = -1;

	int connectPeer();

	public:
		int initialize();

		Connect(in_addr ip){
			Conf :: initConf("init.conf");
			Server server;
			Network nt;
			if((this->ssoc = nt.listenTcp(nt.bindTcp(nt.getTcpSocket(), &listenPort))) < 0) {SYSLOG;}
			if(this->listenPort < 0) {SYSLOG;}
			if((this->a_book = server.getAddrBook(nt.getTcpSocket(), ip, this->listenPort)) == NULL) {SYSLOG;}
		}

};	

int Connect :: connectPeer(){
	Network nt;
	int loop = 0;
	while(loop < this->a_book->size){
		if((this->a_book->clientSockets[loop] = nt.connectTcp(nt.getTcpSocket(),
			this->a_book->a_list[loop].addr, this->a_book->a_list[loop].port)) < 0) { SYSLOG; continue;}

		char msg[100] = "hello\0";
		send(soc[loop], (void*) msg, sizeof(msg), 0); 
		loop++;
	}

	Sys :: log("message send to all address from addr_list");

	return loop;
}

int Connect :: initialize(){

	Network nt;
	AddrList al;
	al.print(this->a_book);

	connectPeer();
	//this->ssoc = nt.listenTcp(nt.bindTcp(nt.getTcpSocket()));
	Sys :: log("Waiting For Node"); 

	int loop = 0;
	while(loop < Conf ::  getInfo<int>(MAC)){
		if((this->csoc[loop] = nt.acceptTcp(this->ssoc)) < 0) {SYSLOG;}
		else { Sys :: log("new Naber node connected"); }

		//client has to add this address in his book also

		char msg[100] = { '\0' };
		int recvBytes = 0;
		if((recvBytes = recv(this->csoc[loop], (void*) msg, sizeof(msg), 0)) < 0) {SYSLOG;}
		else { 	Sys :: log("message from node :");}
		loop++;
	}
	
	Sys :: log("p2p connection listening stop");

	return ssoc;
}
