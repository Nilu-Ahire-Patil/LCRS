#pragma once
#include "conf.h"
#include "network.h"

#define DEFAULT_CONFIGURATION_FILE_PATH "init.conf"

class Connect : public Conf {
//	addr_book* a_book = NULL;
//	int* soc = new int[Conf :: getInfo<int>(MAC)]{ 0 };
//	int* csoc = new int[Conf :: getInfo<int>(MAC)]{ 0 };
	int soc = -1;			//tcp listening socket

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

		Connect(){
			Network nt;
			if((this->soc = nt.setTcpListenPort()) < 0){ STOP(ERROR, ""); }
			if(Conf :: getInfo<unsigned short>(LP) == 0){ STOP(ERROR, ""); }
			if(nt.updateAddrSet(Conf :: getInfo<unsigned short>(LP)) < 0){ STOP(ERROR, ""); }
		}

		Connect(const char* confFilePath){
			Conf :: initConf(confFilePath);
			Connect();
		}
};	

int Connect :: initialize(){
	Network nt;
	nt.receveAndProcessTcp(this->soc);
	SYSLOG(INFO, "p2p connection listening stop");

	return soc;
}

