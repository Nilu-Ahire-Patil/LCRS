//#pragma once

//#include "server.h"

#include "conf.h"
//#include "addrList.h"
#include "network.h"
#define DEFAULT_CONFIGURATION_FILE_PATH "init.conf"

class Connect : public Conf {
//	addr_book* a_book = NULL;
//	int* soc = new int[Conf :: getInfo<int>(MAC)]{ 0 };
//	int* csoc = new int[Conf :: getInfo<int>(MAC)]{ 0 };
	int soc = -1;			//tcp listening socket
	unsigned short listenPort = -1;

//	int connectPeer();

	public:
		int initialize();

	/*	Connect(in_addr ip, char* confFileName){
			Conf :: initConf(confFileName);
			Server server;
			Network nt;
			if((this->ssoc = nt.listenTcp(nt.bindTcp(nt.getTcpSocket(), &listenPort))) < 0) {SYSLOG;}
			if(this->listenPort < 0) {SYSLOG;}
			if((this->a_book = server.getAddrBook(nt.getTcpSocket(), ip, this->listenPort)) == NULL) {SYSLOG;}
		}
	*/

		Connect() : Connect(DEFAULT_CONFIGURATION_FILE_PATH){}

		Connect(const char* confFilePath){
			Conf :: initConf(confFilePath);
			Network nt;
			if((this->soc = nt.listenTcp(nt.bindTcp(&listenPort))) < 0){ SYSLOG; STOP; }
			if(this->listenPort < 0){ SYSLOG; STOP; }
			if(nt.updateAddrSet(this->listenPort) < 0){ SYSLOG; STOP; }
		}

};	
/*
int Connect :: connectAllPeer(){
	Network nt;



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
*/
int Connect :: initialize(){

	Network nt;
//	AddrList al;
//	al.print(this->a_book);

//	connectPeer();
	//this->ssoc = nt.listenTcp(nt.bindTcp(nt.getTcpSocket()));
	USRLOG("Waiting For Node"); 

	int loop = 0;
	while(loop < Conf ::  getInfo<int>(MAC)){
		int csoc = -1;
		if((csoc = nt.acceptTcp(this->soc)) < 0) {SYSLOG;}
		else { USRLOG("new Naber node connected"); }

		//client has to add this address in his book also

		char msg[100] = { '\0' };
		int recvBytes = 0;
		if((recvBytes = recv(csoc, (void*) msg, sizeof(msg), 0)) < 0) {SYSLOG;}
		else { USRLOG("message from node :");}
		loop++;
	}
	
	USRLOG("p2p connection listening stop");

	return soc;
}

