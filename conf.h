#pragma once

#include <variant>
#include <vector>
#include <map>
#include <set>
#include "sys.h"

using namespace std;

//#include <cstring>
//#include <iostream>
#define CONF_DATA_TYPE	map<string, variant<string, int, double, unsigned short, vector<int>, set<int>, set<string>>>
#define MAX_PACKET_SIZE 1024
#define MAX_LINE_LENGTH 100
#define DELIMITER 	" = "
#define LP 		"listning_port"

/*-------------------------------------------------------------------------------------------------*/

//Key Words and default values
//#define MAC 		"max_add_capacity"
//#define V_MAC 		1024

#define MCB 		"max_con_buff"
#define V_MCB 		100 

#define TCP_PORTS 	"tcp_ports"
#define V_TCP_PORTS 	9090, 9091, 9092

#define UDP_PORTS 	"udp_ports"
#define V_UDP_PORTS 	9080, 9081, 9082

#define BGA 		"broadcast_group_address"
#define V_BGA 		"239.0.1.1", "239.0.1.2"

/*-------------------------------------------------------------------------------------------------*/

class Conf : public Sys{
	private:
   		static CONF_DATA_TYPE confData;

		static CONF_DATA_TYPE loadDefaultConfig(){
   			static CONF_DATA_TYPE confData;

	    	//	confData[MAC] = V_MAC;
			confData[MCB] = V_MCB;
			confData[TCP_PORTS] = set<int>{ V_TCP_PORTS };
			confData[UDP_PORTS] = set<int>{ V_UDP_PORTS };
			confData[BGA] = set<string>{ V_BGA };
			confData[LP] = (unsigned short) 0;
			return confData;
		}

		static int initInt(const string& key, char* line){
			string format = key + DELIMITER;
			string fullFormat = format + "%d";

			int value = -1;
        		if(strncmp(line, format.c_str(), format.length()) == 0){
        	    		sscanf(line, fullFormat.c_str(), &value);
				if(value >= 0){ setInfo<int>(key, value); }
				else{ return -1; }
        		}
			else{ return -1; }

			return 0;
		}

		static int initString(const string& key, char* line){
			string format = key + DELIMITER;
			string fullFormat = format + "%s";

			char value[MAX_LINE_LENGTH] = { '\0' };
        		if(strncmp(line, format.c_str(), format.length()) == 0){
        	    		sscanf(line, fullFormat.c_str(), &value);
				if(value[0] != '\0'){ setInfo<string>(key, value); }
				else{ return -1; }
        		}
			else{ return -1; }

			return 0;
		}

		static int initIntSet(const string& key, char* line){
			string format = key + DELIMITER + "{";
			string fullFormat = format;

			set<int> value;
        		if(strncmp(line, format.c_str(), format.length()) == 0){
            			char *token = strtok(line+format.length(), "{}, "); 
            			while(token != NULL){
					int val = -1;
                			sscanf(token, "%d", &val);
					if(val > 0){ value.insert(val); }
                			token = strtok(NULL, "{}, ");
				}
				if(value.size() > 0){ setInfo<set<int>>(key, value); SYSLOG(INFO, "int set loaded"); }
			}
			else{ return -1; }
			return 0;
         	}

		static int initStringSet(const string& key, char* line){
			string format = key + DELIMITER + "{";
			string fullFormat = format;

			set<string> value;
        		if(strncmp(line, format.c_str(), format.length()) == 0){
            			char *token = strtok(line + format.length(), "{}, "); 
            			while(token != NULL){
					char* val =  new char[MAX_LINE_LENGTH]{ '\0' };
                			sscanf(token, "%s", val);
					if(val[0] != '\0'){ value.insert(val); }
					delete[] val;
                			token = strtok(NULL, "{}, ");
				}
				if(value.size() > 0){ setInfo<set<string>>(key, value); SYSLOG(INFO, "string set loaded"); }
			}
			else{ return -1; }
			return 0;
            }

	public:

		static int initConf(const string& confFilePath){
			if(confFilePath.empty()){ STOP(ERROR, ""); }

			FILE *confFile;
       			if((confFile = fopen(confFilePath.c_str(), "r")) == NULL){ STOP(ERROR, ""); }

			char line[MAX_LINE_LENGTH] = { '\0' };
    			while(fgets(line, sizeof(line), confFile) != NULL){

				char *comment_pos = NULL;
        			if((comment_pos = strstr(line, "//")) != NULL){ *comment_pos = '\0'; }

			//	if(initInt(MAC, line) >= 0){ continue; }
				if(initInt(MCB, line) >= 0){ continue; }
				else if(initIntSet(TCP_PORTS, line) >= 0){ continue; }
				else if(initIntSet(UDP_PORTS, line) >= 0){ continue; }
				else if(initStringSet(BGA, line) >= 0){ continue; }
			}
			return 0;
		}

		template <typename T>
		static T getInfo(const string& key){ return get<T>(confData[key]); }

		template <typename T>
		static void setInfo(const string& key, T value){ confData[key] = value; }

		static addr_book a_book;
};

/*-------------------------------------------------------------------------------------------------*/

addr_book Conf :: a_book;

CONF_DATA_TYPE Conf::confData = Conf::loadDefaultConfig();

/*-------------------------------------------------------------------------------------------------*/
