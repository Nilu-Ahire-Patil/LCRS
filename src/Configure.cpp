#ifndef CONFIG_IMPL_H
#define CONFIG_IMPL_H

#ifndef CONFIG_H
#include "../include/Configure.h"	//interface
#endif

#ifndef ADDRBOOK_H
#include "../include/AddrBook.h"	//addr_book
#endif

#ifndef SYS_H
#include "../include/Sys.h"		//SYSLOG, STOP
#endif

#include <unistd.h> 	// close
#include <sys/ioctl.h>	// ioctl
#include <net/if.h>	// ifreq
#include <cstring>	//strtok

/*-------------------------------------------------------------------------------------------------*/

std::unordered_map<std::string, std::variant<std::string, int, double, unsigned short, 
	std::vector<int>, std::vector<std::string>, 
	std::set<int>, std::set<std::string>>> Conf::confData; 

addr_book Conf::a_book;

/*-------------------------------------------------------------------------------------------------*/

std::unordered_map<std::string, std::variant<std::string, int, double, unsigned short, 
		std::vector<int>, std::vector<std::string>, 
		std::set<int>, std::set<std::string>>> Conf::loadDefaultConfig(){

   	std::unordered_map<std::string, std::variant<std::string, int, double, unsigned short, 
		std::vector<int>, std::vector<std::string>, 
		std::set<int>, std::set<std::string>>> confData;
	
	confData[MCB] = V_MCB;
	confData[TCP_PORTS] = std::set<int>{ V_TCP_PORTS };
	confData[UDP_PORTS] = std::set<int>{ V_UDP_PORTS };
	confData[BGA] = std::set<std::string>{ V_BGA };
	confData[LP] = (unsigned short) 0;
	confData[NET_INTERFACES] = std::vector<std::string>{ V_NET_INTERFACES };
	return confData;
}

int Conf::initInt(const std::string& key, char* line){
	std::string format = key + DELIMITER;
	std::string fullFormat = format + "%d";
	int value = -1;
	if(strncmp(line, format.c_str(), format.length()) == 0){
    		sscanf(line, fullFormat.c_str(), &value);
		if(value >= 0){ setInfo<int>(key, value); return 0; }
	}
	return -1;
}

int Conf::initString(const std::string& key, char* line){
	std::string format = key + DELIMITER;
	std::string fullFormat = format + "%s";

	char value[MAX_LINE_LENGTH] = { '\0' };
	if(strncmp(line, format.c_str(), format.length()) == 0){
    		sscanf(line, fullFormat.c_str(), &value);
		if(value[0] != '\0'){ setInfo<std::string>(key, value); return 0; }
	}
	return -1; 
}

int Conf::initIntSet(const std::string& key, char* line){
	std::string format = key + DELIMITER + "{";

	if(strncmp(line, format.c_str(), format.length()) != 0){ return -1; }

	std::set<int> value;
	char *token = strtok(line+format.length(), "{}, "); 
        while(token != NULL){
		int val = -1;
               	sscanf(token, "%d", &val);
		if(val > 0){ value.insert(val); }
               	token = strtok(NULL, "{}, ");
	}

	if(value.size() > 0){ setInfo<std::set<int>>(key, value); return 0;}
	else { return -1; }

	return 0;
}

int Conf::initStringSet(const std::string& key, char* line){
	std::string format = key + DELIMITER + "{";

       	if(strncmp(line, format.c_str(), format.length()) != 0){ return -1; }

	std::set<std::string> value;
	char *token = strtok(line + format.length(), "{}, ");
	while(token != NULL){
		char val[MAX_LINE_LENGTH] = { '\0' };
		sscanf(token, "%s", val);
		if(val[0] != '\0'){ value.insert(val); }
                token = strtok(NULL, "{}, ");
	}

	if(value.size() > 0){ setInfo<std::set<std::string>>(key, value); return 0;}
	else { return -1; }
}

int Conf::initStringVector(const std::string& key, char* line){
	std::string format = key + DELIMITER + "{";

	if(strncmp(line, format.c_str(), format.length()) != 0){ return -1; }

	std::vector<std::string> value;
	char *token = strtok(line + format.length(), "{}, "); 

        while(token != NULL){
		char val[MAX_LINE_LENGTH] = { '\0' };
         	sscanf(token, "%s", val);
		if(val[0] != '\0'){ value.push_back(val); }
                token = strtok(NULL, "{}, ");
	}

	if(value.size() > 0){ setInfo<std::vector<std::string>>(key, value); return 0; }
	else { return -1; }
}


std::string Conf::getInterfaceMacAddress(const std::string& interface) {
	int soc = -1;
	if((soc = socket(AF_INET, SOCK_DGRAM, 0)) < 0){ STOP(ERROR, "socket not set properly"); }

	ifreq ifr;
	// copy interface name 
	strncpy(ifr.ifr_name, interface.c_str(), IFNAMSIZ - 1);
	// request for ip address of that interface
	if (ioctl(soc, SIOCGIFHWADDR, &ifr) != 0) {
		close(soc);
		SYSLOG(WARN, std::string(interface) + ": fail to get interface mac id"); 
		return nullptr;
	}

	close(soc);
			
	return std::string(ifr.ifr_hwaddr.sa_data);
}

void Conf::initSysId(){
	std::vector<std::string> interfaces = getInfo<std::vector<std::string>>(NET_INTERFACES);

	if(interfaces.empty()){ STOP(ERROR, "fail to find interfaces"); }

	std::vector<std::string>::iterator it = interfaces.begin();
	while(it != interfaces.end()){
		std::string sysId = getInterfaceMacAddress(*it);
		if(!sysId.empty()){
			setInfo<std::string>(SYS_ID, sysId);

			const unsigned char* sysid = (const unsigned char*) sysId.c_str();
			char sysid_str[50]{ '\0' };
    			sprintf(sysid_str, "%s:%02X:%02X:%02X:%02X:%02X:%02X ",
				(*it).c_str(), sysid[0], sysid[1], sysid[2], sysid[3], sysid[4], sysid[5]);

			SYSLOG(SUCCESS, std::string(sysid_str) + "system Id set");
			break;
		}
		++it;
	}
	if(it == interfaces.end()){ STOP(ERROR, "fail to set system id"); }
}

int Conf::initConf(){ 
	confData = loadDefaultConfig();
	// set unique system id for uniquely identify in network
	initSysId();
	return 0;
}

int Conf::initConf(const std::string& confFilePath){
	if(confFilePath.empty()){ STOP(ERROR, "empty file path"); }

	FILE *confFile;
	if((confFile = fopen(confFilePath.c_str(), "r")) == NULL){ STOP(ERROR, ""); }

	// load default configuration
	confData = loadDefaultConfig();

	char line[MAX_LINE_LENGTH] = { '\0' };
	while(fgets(line, sizeof(line), confFile) != NULL){

		// handling comment
		char *comment_pos = NULL;
		if((comment_pos = strstr(line, "//")) != NULL){ *comment_pos = '\0'; }

		// comparing with all available keys
		if(initInt(MCB, line) >= 0){ continue; }
		else if(initIntSet(TCP_PORTS, line) >= 0){ continue; }
		else if(initIntSet(UDP_PORTS, line) >= 0){ continue; }
		else if(initStringSet(BGA, line) >= 0){ continue; }
		else if(initStringVector(NET_INTERFACES, line) >= 0){ continue; }
	}

	// set unique system id for uniquely identify in network
	initSysId();

	return 0;
}

/*-------------------------------------------------------------------------------------------------*/
#endif
