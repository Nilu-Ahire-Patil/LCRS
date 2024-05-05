#pragma once

#include "addrList.h"

//#include <arpa/inet.h>
//#include <set>
//#include <iostream>
//#include <cstring>
//#include "mConfig.h"

using namespace std;

class Network {

	public:
		int getTcpSocket();

		int bindTcp(int);
		int bindTcp(int, unsigned short*);

		int listenTcp(int);
		int acceptTcp(int);

		int connectTcp(int, in_addr);
		int connectTcp(int, in_addr, unsigned short int);
};

int Network :: getTcpSocket(){

	int soc = -1;
	if((soc = socket(AF_INET, SOCK_STREAM, 0)) < 0) { SYSLOG; return -1;}

	int optval = 1;
	if(setsockopt(soc, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0) { SYSLOG;}

	return soc;
}

int Network :: bindTcp(int soc, unsigned short* listning_port){
	if(soc < 0) return soc;
 
	set<int> ports = Conf :: getInfo<set<int>>(PORTS);
	set<int>::iterator it = ports.begin();
	while(it != ports.end()){
		sockaddr_in add;
		memset(&add, 0, sizeof(sockaddr_in));
		add.sin_family = AF_INET;
		add.sin_port = htons(*it);
		add.sin_addr.s_addr = INADDR_ANY;

		if(bind(soc, (sockaddr*) &add, sizeof(add)) < 0) { SYSLOG; it++;}
		else {
			*listning_port = *it;
			Sys :: log("listning port configure to : " + *it);
			cout << "listning port " << *it << endl;
			break;
		}
	}
	return soc;
}

int Network :: bindTcp(int soc){
	if(soc < 0) return soc;

	set<int> ports = Conf :: getInfo<set<int>>(PORTS);
	set<int>::iterator it = ports.begin();
	while(it != ports.end()){
		sockaddr_in add;
		memset(&add, 0, sizeof(sockaddr_in));
		add.sin_family = AF_INET;
		add.sin_port = htons(*it);
		add.sin_addr.s_addr = INADDR_ANY;

		if(bind(soc, (sockaddr*) &add, sizeof(add)) < 0) { SYSLOG; it++;}
		else {	Sys :: log("port configure " + *it); break;}
	}
	if(ports.empty()){ SYSLOG;}
	return soc;
}

int Network :: listenTcp(int soc){
	if(soc < 0) return soc;

	if(listen(soc, Conf :: getInfo<int>(MCB)) < 0) { SYSLOG;}

	return soc;
}

int Network :: acceptTcp(int soc){
	if(soc < 0) return soc;

	int csoc = -1;
	sockaddr_in c_addr;
	socklen_t c_addr_len = sizeof(c_addr);

	if((csoc = accept(soc, (struct sockaddr*) &c_addr, &c_addr_len)) < 0) { SYSLOG;}

	return csoc;
}

int Network :: connectTcp(int soc, in_addr ip){
	if(soc < 0) return soc;

	set<int> ports = Conf :: getInfo<set<int>>(PORTS);
	set<int>::iterator it = ports.begin();
	while(it != ports.end()){
		if(connectTcp(soc, ip, *it) < 0) { SYSLOG; ++it;}
		else{ break;}
	}

	return soc;
}

int Network :: connectTcp(int soc, in_addr ip, unsigned short int port){
	if(soc < 0) return soc;

	sockaddr_in s_addr;
	socklen_t s_addr_len = sizeof(s_addr);

	memset(&s_addr, 0, sizeof(sockaddr_in));
	s_addr.sin_family = AF_INET;
	s_addr.sin_port = htons(port);
	s_addr.sin_addr = ip;

	if(connect(soc,(sockaddr*) &s_addr, s_addr_len) < 0) { SYSLOG;}

	return soc;
}
