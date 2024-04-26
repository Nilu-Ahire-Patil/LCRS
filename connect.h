#include "server.h"

class Connect {
	addr_list* a_list = NULL;
	int soc[Conf :: max_add_capacity] = { -1 };
	int csoc[Conf :: max_add_capacity] = { -1 };
	int cCount = 0;
	int ssoc = -1;

	int connectPeer();

	public:
		int initialize();

		Connect(in_addr ip){
			Server server;
			Network nt;
			if((this->a_list = server.getAddrList(nt.getTcpSocket(), ip)) == NULL) { Sys :: log(__func__,__LINE__);}
		}
};	

int Connect :: connectPeer(){
	int loop = 0;
	Network nt;
	while(loop < this->a_list->size - 1){
		if((this->soc[loop] = nt.connectTcp(nt.getTcpSocket(), this->a_list->list[loop])) < 0) { Sys :: log(__func__,__LINE__); continue;}
		char msg[100] = "hello\0";
		send(soc[loop], (void*) msg, sizeof(msg), 0); 
		loop++;
	}

	Sys :: log("messagee send to all address from addr_list");

	return loop;
}

int Connect :: initialize(){

	Network nt;
	AddrList al;
	al.print(this->a_list);

	connectPeer();
	this->ssoc = nt.listenTcp(nt.bindTcp(nt.getTcpSocket()));

	while(1){
		char msg[100] = { '\0' };
		cout<<"waiting to smone conect"<<endl;
		if((this->csoc[cCount] = nt.acceptTcp(this->ssoc)) < 0) { Sys :: log(__func__,__LINE__); continue;}
		else { Sys :: log("new nabur node connected"); }

		int recvBytes = 0;
		if((recvBytes = recv(this->csoc[cCount], (void*) msg, sizeof(msg), 0)) < 0) { Sys :: log(__func__,__LINE__);}
		else { 	Sys :: log("message from node");
			cout<< msg << endl;
	       	}

		cCount++;
	}
	
	Sys :: log("p2p connetion listning stop");

	return ssoc;
}
