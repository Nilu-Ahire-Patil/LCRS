#ifndef CONFIG_IMPL_H
#define CONFIG_IMPL_H

#include "Configure.h"		// interface
#include "Sys.h"		// SYSLOG, STOP
#include "Network.h"

#include <unistd.h> 		// close
#include <cstring>		// strtok
				// strncpy
#include <uuid/uuid.h> 		// uuid_t

/*-------------------------------------------------------------------------------------------------*/

// reserve memory for conffig data
std::unordered_map<std::string, std::variant<std::string, int, double, unsigned short, 
	std::vector<int>, std::vector<std::string>, 
	std::set<int>, std::set<std::string>>> Conf::confData; 

// reserve memory for address book
std::unordered_map<sys_id, n_addr, sys_id_hash> Conf::addr_book;

// system id
sys_id Conf::s_id;
/*-------------------------------------------------------------------------------------------------*/

// load default configuration in confData variable
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
	//confData[NET_INTERFACES] = std::vector<std::string>{ V_NET_INTERFACES };
	
	// set unique system id for uniquely identify in network
	//getSysId();

	return confData;
}

// finds integer value in line and insert in configuration data with provided key
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

// finds string value in line and insert in configuration data with provided key
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

// finds integer set from line and insert in configuration data with provided key
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

// finds string set from line and insert in configuration data with provided key
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

// finds string vector from line and insert in configuration data with provided key
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

// initialise system id
int Conf::initSysId(){
    	// DNS namespace UUID
	uuid_t namespace_uuid; 
    	const char* namespace_dns = NAMESPACE_DNS;
	uuid_parse(namespace_dns, namespace_uuid); // Parse the DNS namespace UUID

	Network nt;
   	std::string mac_addr = nt.getMacAddress();
    	if(mac_addr.empty()){ STOP(ERROR, "fail to intialise id"); }

    	// Generate the UUID v5
    	uuid_t uuid; 
    	uuid_generate_sha1(uuid, namespace_uuid, mac_addr.c_str(), strlen(mac_addr.c_str()));

	// asign system value to an static variable
	s_id = sys_id(uuid);

    	SYSLOG(INFO, "sysyem id : " + s_id.str_id()); 

	return 0;
}

// initialise system id
const sys_id& Conf::getSysId(){

	// innitialise system id
	if(s_id.isEmpty() == 1){ initSysId(); }
	
	return s_id;
}

// initialise system with default configurstion
int Conf::initConf(){ 
	return initConf(DEFAULT_CONFIG_PATH);
}

// initialise system with configuration file
int Conf::initConf(const std::string& confFilePath){
	// initialise system with default configurstion
	confData = loadDefaultConfig();

	// innitialise system id
	initSysId();

	// wecan print system id here

	if(confFilePath.empty()){ SYSLOG(ERROR, "empty file path"); }

	FILE *confFile;
	if((confFile = fopen(confFilePath.c_str(), "r")) == NULL){ 
		SYSLOG(ERROR, confFilePath + ":runing on internal configuration"); 
		return 1;
	}
	
	SYSLOG(SUCCESS, confFilePath + " file loaded"); 

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
	// 	else if(initStringVector(NET_INTERFACES, line) >= 0){ continue; }
	}

	return 0;
}

/*-------------------------------------------------------------------------------------------------*/
#endif
