#ifndef CONNECT_IMPL_H
#define CONNECT_IMPL_H

#include "Connect.h"		// interface
#include "Sys.h"		// Network
#include "Configure.h"		// Conf
#include "Network.h"		// Network

#include <thread>		// thread

/*-------------------------------------------------------------------------------------------------*/

// connect with other nodes with default configuration
Connect::Connect(){
	Conf :: initConf();
	Network nt;
	if((this->soc = nt.setTcpListenPort()) < 0){ STOP(ERROR, ""); }
	if(Conf :: getInfo<unsigned short>(LP) == 0){ STOP(ERROR, ""); }
	if(nt.updateAddrSet(Conf :: getInfo<unsigned short>(LP)) < 0){ STOP(ERROR, ""); }
}

// connect with other node with user provided configuration
Connect::Connect(const char* confFilePath){
	Conf :: initConf(confFilePath);
	Network nt;
	if((this->soc = nt.setTcpListenPort()) < 0){ STOP(ERROR, ""); }
	if(Conf :: getInfo<unsigned short>(LP) == 0){ STOP(ERROR, ""); }
	if(nt.updateAddrSet(Conf :: getInfo<unsigned short>(LP)) < 0){ STOP(ERROR, ""); }
}

// initialize tcp base packet reply
int Connect::initialize(){
	//replying tcp packets
	std::thread tcpReplying([this](){ 
			Network nt;
			nt.receveAndProcessTcp(this->soc); 
		});
	tcpReplying.detach();

	return soc;
}

/*-------------------------------------------------------------------------------------------------*/
#endif

