int main(){
	return 0;
}

#ifndef MAIN
#define MAIN

#include "Log.h"		// getCurrentDateString
#include "Connect.h"	// Connect
#include "Packet.h"		// Packet
#include "Protocol.h"	// Protocol
#include "File.h"		// store

#include <string>		// string
#include <iostream>		// cin, cout
#include <cstdio>		// getchar
#include <unordered_map>
enum class CMD {
	UNKNOWN,
	STORE,
	MESSAGE,
	LOG,
	EXIT
};

CMD str_to_cmd(const std::string& cmd){
	static const std::unordered_map<std::string, CMD> commandMap = {
		{"store", CMD::STORE},
		{"msg", CMD::MESSAGE},
		{"log", CMD::LOG},
        	{"exit", CMD::EXIT},
        	{"q", CMD::EXIT}
	};
	auto it = commandMap.find(cmd);
    	if(it != commandMap.end()){ return it->second; }
        return CMD::UNKNOWN;
}

int main1(int argc, char** argv)
{
	// Redirect stderr to a file
	/*
	FILE* file = nullptr;
	std::string logFilePath = "log/" + Log::getCurrentDateString() +".log";
	if((file = freopen(logFilePath.c_str(), "a", stderr)) == nullptr){
      	std::cerr << "Failed to redirect stderr to file" << std::endl;
	}
	*/

	if(argc < 2){ Connect cn; cn.initialize(); }
	else { Connect cn(argv[1]); cn.initialize(); }

	Protocol pro; 
	while(1){	
		std::cout << "\n>>> ";
		std::string command;
		std::cin >> command;

		switch(str_to_cmd(command)){

			// exit command
			case CMD::EXIT: { return 0; }

			case CMD::LOG: { 
				FILE* file = nullptr;
				std::string logFilePath = "log/" + Log::getCurrentDateString() +".log";
				if((file = freopen(logFilePath.c_str(), "a", stderr)) == nullptr){
      					std::cerr << "Failed to redirect stderr to file" << std::endl;
				}
				break;
			}

			// send message using tcp
			case CMD::MESSAGE: {
				std::string ip;
				unsigned int port;
				std::string msg;
				std::cin >> ip;
				std::cin >> port;
				std::cin >> msg;
				n_addr addr(ip, port);
				pro.repTcpMessage(addr, msg);
				break;
			}

			case CMD::STORE : {
				std::string path;
				std::cin >> path;
				file f;
				f.store(path);
				break;
			}

			case CMD::UNKNOWN: {
      				std::cout << "command not found: " << command << std::endl;
				break;
			}
			default: { std::cout << "command not found: " << command << std::endl; }
		}
	}

	return 0;
}	
#endif
