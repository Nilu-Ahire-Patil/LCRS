#pragma once

#include <variant>
#include <vector>
#include <map>
#include <set>
#include <unistd.h> 	// for close
#include <sys/ioctl.h>	// for ioctl
#include <net/if.h>	// for struct ifreq
#include "sys.h"

using namespace std;

//#include <cstring>
//#include <iostream>
#define CONF_DATA_TYPE	map<string, variant<string, int, double, unsigned short, vector<int>, vector<string>, set<int>, set<string>>>
#define MAX_PACKET_SIZE 1024
#define MAX_LINE_LENGTH 100
#define DELIMITER 	" = "

#define LP 		"LISTNING_PORT"
#define SYS_ID		"SYS_ID"

/*-------------------------------------------------------------------------------------------------*/

//Key Words and default values

#define MCB 		"max_con_buff"
#define V_MCB 		100 

#define TCP_PORTS 	"tcp_ports"
#define V_TCP_PORTS 	9090, 9091, 9092

#define UDP_PORTS 	"udp_ports"
#define V_UDP_PORTS 	9080, 9081, 9082

#define BGA 		"broadcast_group_address"
#define V_BGA 		"239.0.1.1", "239.0.1.2"

// interface set for mac address
// order of interfaces matters
#define NET_INTERFACES	"network_interfaces"
#define V_NET_INTERFACES "wlp3s0", "enp0s31f6" 

/*-------------------------------------------------------------------------------------------------*/

class Conf : public Sys {
	private:
   		static CONF_DATA_TYPE confData;

		static CONF_DATA_TYPE loadDefaultConfig(){
   			static CONF_DATA_TYPE confData;

			confData[MCB] = V_MCB;
			confData[TCP_PORTS] = set<int>{ V_TCP_PORTS };
			confData[UDP_PORTS] = set<int>{ V_UDP_PORTS };
			confData[BGA] = set<string>{ V_BGA };
			confData[LP] = (unsigned short) 0;
			confData[NET_INTERFACES] = vector<string>{ V_NET_INTERFACES };
			return confData;
		}

		static int initInt(const string& key, char* line){
			string format = key + DELIMITER;
			string fullFormat = format + "%d";

			int value = -1;
        		if(strncmp(line, format.c_str(), format.length()) == 0){
        	    		sscanf(line, fullFormat.c_str(), &value);
				if(value >= 0){ 
					setInfo<int>(key, value); 
					return 0;
				}

				return -1;
        		}

			return -1;
		}

		static int initString(const string& key, char* line){
			string format = key + DELIMITER;
			string fullFormat = format + "%s";

			char value[MAX_LINE_LENGTH] = { '\0' };
        		if(strncmp(line, format.c_str(), format.length()) == 0){
        	    		sscanf(line, fullFormat.c_str(), &value);

				if(value[0] != '\0'){ 
					setInfo<string>(key, value); 
					return 0;
				}

				return -1; 
        		}

			return -1; 
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

					if(val > 0){ 
						value.insert(val); 
					}

                			token = strtok(NULL, "{}, ");
				}

				if(value.size() > 0){ 
					setInfo<set<int>>(key, value);
				//	SYSLOG(INFO, "int set loaded"); 
				}

				return 0;
			}
			return -1;
         	}

		static int initStringSet(const string& key, char* line){
			string format = key + DELIMITER + "{";
			string fullFormat = format;

			set<string> value;
        		if(strncmp(line, format.c_str(), format.length()) == 0){
            			char *token = strtok(line + format.length(), "{}, "); 

            			while(token != NULL){
					char val[MAX_LINE_LENGTH] = { '\0' };
                			sscanf(token, "%s", val);

					if(val[0] != '\0'){ 
						value.insert(val); 
					}

                			token = strtok(NULL, "{}, ");
				}

				if(value.size() > 0){ 
					setInfo<set<string>>(key, value); 
					//SYSLOG(INFO, "string set loaded"); 
				}

				return 0;
			}

			return -1; 
		}

		static int initStringVector(const string& key, char* line){
			string format = key + DELIMITER + "{";
			string fullFormat = format;

			vector<string> value;
        		if(strncmp(line, format.c_str(), format.length()) == 0){
            			char *token = strtok(line + format.length(), "{}, "); 

            			while(token != NULL){
					char val[MAX_LINE_LENGTH] = { '\0' };
                			sscanf(token, "%s", val);

					if(val[0] != '\0'){ 
						value.push_back(val); 
					}

                			token = strtok(NULL, "{}, ");
				}

				if(value.size() > 0){ 
					setInfo<vector<string>>(key, value); 
					//SYSLOG(INFO, "string set loaded"); 
				}

				return 0;
			}

			return -1; 
		}

		static string getInterfaceMacAddress(const string& interface) {
			int soc = -1;
			if((soc = socket(AF_INET, SOCK_DGRAM, 0)) < 0){ 
				STOP(ERROR, "socket not set properly"); 
			}

    			ifreq ifr;
			// copy interface name 
    			strncpy(ifr.ifr_name, interface.c_str(), IFNAMSIZ - 1);
			// request for ip address of that interface
    			if (ioctl(soc, SIOCGIFHWADDR, &ifr) != 0) {
        			close(soc);
				SYSLOG(WARN, string(interface) + ": fail to get interface mac id"); 
				return "";
			}

    			close(soc);
			
			return string(ifr.ifr_hwaddr.sa_data);
		}

		static void initSysId(){
			vector<string> interfaces = getInfo<vector<string>>(NET_INTERFACES);

			if(interfaces.empty()){ 
				STOP(ERROR, "fail to find interfaces"); 
			}

			vector<string>::iterator it = interfaces.begin();
			while(it != interfaces.end()){
				string sysId = getInterfaceMacAddress(*it);
				if(!sysId.empty()){
					setInfo<string>(SYS_ID, sysId);

					const unsigned char* sysid = (const unsigned char*) sysId.c_str();
					char sysid_str[50]{ '\0' };
    					sprintf(sysid_str, "%s:%02X:%02X:%02X:%02X:%02X:%02X ", (*it).c_str(), sysid[0], sysid[1], sysid[2], sysid[3], sysid[4], sysid[5]);

					SYSLOG(SUCCESS, string(sysid_str) + "system Id set");
					break;
				}
				++it;
			}

			if(it == interfaces.end()){ 
				STOP(ERROR, "fail to set system id"); 
			}
		}



	public:

		static int initConf(){
			confData = loadDefaultConfig();
			// set unique system id for uniquely identify in network
			initSysId();
			return 0;
		}

		static int initConf(const string& confFilePath){
			if(confFilePath.empty()){ 
				STOP(ERROR, "empty file path"); 
			}

			FILE *confFile;
       			if((confFile = fopen(confFilePath.c_str(), "r")) == NULL){ 
				STOP(ERROR, ""); 
			}

			// load default configuration
			confData = loadDefaultConfig();

			char line[MAX_LINE_LENGTH] = { '\0' };
    			while(fgets(line, sizeof(line), confFile) != NULL){

				// handling comment
				char *comment_pos = NULL;
        			if((comment_pos = strstr(line, "//")) != NULL){ 
					*comment_pos = '\0'; 
				}

				// comparing with all available keys
				if(initInt(MCB, line) >= 0){ 
					continue; 
				}
				else if(initIntSet(TCP_PORTS, line) >= 0){ 
					continue; 
				}
				else if(initIntSet(UDP_PORTS, line) >= 0){ 
					continue; 
				}
				else if(initStringSet(BGA, line) >= 0){ 
					continue; 
				}
				else if(initStringVector(NET_INTERFACES, line) >= 0){ 
					continue; 
				}
			}

			// set unique system id for uniquely identify in network
			initSysId();

			return 0;
		}

		template <typename T>
		static T getInfo(const string& key){ 
			return get<T>(confData[key]); 
		}

		template <typename T>
		static void setInfo(const string& key, T value){ 
			confData[key] = value; 
		}

		static addr_book a_book;
};

/*-------------------------------------------------------------------------------------------------*/

CONF_DATA_TYPE Conf::confData;// = Conf::loadDefaultConfig();

addr_book Conf::a_book;

/*-------------------------------------------------------------------------------------------------*/
