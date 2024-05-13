#pragma once

#include <arpa/inet.h>
#include <variant>
#include <vector>
#include <map>
#include <set>
#include "sys.h"

//#include <cstring>
//#include <iostream>

//Key Words
#define MAX_LINE_LENGTH 100
#define DELIMITER 	" = "

#define MAC 		"max_add_capacity"
#define MCB 		"max_con_buff"
#define PCOUNT 		"port_count"
#define TCP_PORTS 	"tcp_ports"
#define UDP_PORTS 	"udp_ports"
#define BGA 		"broadcast_group_address"

//default Values
#define V_MAC 		24
#define V_MCB 		10 
#define V_PCOUNT	3 
#define V_TCP_PORTS 	9090, 9091, 9092
#define V_UDP_PORTS 	9080, 9081, 9082
#define V_BGA 		"239.0.1.1", "239.0.1.2"

using namespace std;

struct n_addr {
	in_addr ip = { 0 };
	unsigned short port = { 0 };

	bool operator<(const n_addr& other) const {
		return ip.s_addr < other.ip.s_addr || (ip.s_addr == other.ip.s_addr && port < other.port);
	}

	n_addr(in_addr ip, unsigned short port) : ip(ip), port(port) {}
};

struct packet {
	unsigned int flag = { 0 };
};

struct addr_book {
	set<n_addr> addr_set; //address list
				    
	void add(n_addr& oth){ addr_set.insert(oth); } 

	int size(){ return addr_set.size(); }
};

class Conf : public Sys{
	private:

   		static map<string, variant<string, int, double, vector<int>, set<int>, set<string>>> confData;

		static map<string, variant<string, int, double, vector<int>, set<int>, set<string>>> loadDefaultConfig(){
   			static map<string, variant<string, int, double, vector<int>, set<int>, set<string>>> confData;
	    		confData[MAC] = V_MAC;
			confData[MCB] = V_MCB;
			confData[PCOUNT] = V_PCOUNT;
			confData[TCP_PORTS] = set<int>{ V_TCP_PORTS };
			confData[UDP_PORTS] = set<int>{ V_UDP_PORTS };
			confData[BGA] = set<string>{ V_BGA };
			return confData;
		}

		static int initInt(string key, char* line){

			string format = key + DELIMITER;
			string fullFormat = format + "%d";

			int value = -1;
        		if (strncmp(line, format.c_str(), format.length()) == 0){
        	    		sscanf(line, fullFormat.c_str(), &value);
				if(value >= 0){ Conf :: confData[key] = value; }
				else{ return -1; }
        		}
			else{ return -1; }

			return 0;
		}

		static int initString(string key, char* line){

			string format = key + DELIMITER;
			string fullFormat = format + "%s";

			char value[MAX_LINE_LENGTH] = { '\0' };
        		if (strncmp(line, format.c_str(), format.length()) == 0){
        	    		sscanf(line, fullFormat.c_str(), &value);
				if(value[0] != '\0'){ Conf :: confData[key] = value; }
				else{ return -1; }
        		}
			else{ return -1; }

			return 0;
		}

		static int initIntSet(string key, char* line){

			string format = key + DELIMITER + "{";
			string fullFormat = format;

			set<int> value;
        		if(strncmp(line, format.c_str(), format.length()) == 0){
            			char *token = strtok(line+format.length(), "{}, "); 
            			while(token != NULL){
					int val = -1;
                			sscanf(token, "%d", &val);
					cout<<val<<endl;
					if(val > 0){ value.insert(val); }
                			token = strtok(NULL, "{}, ");
				}
				if(value.size() > 0){ confData[key] = value; USRLOG("port list found"); }
			}
			else{ return -1; }
			return 0;
         	}

		static int initStringSet(string key, char* line){

			string format = key + DELIMITER + "{";
			string fullFormat = format;

			set<string> value;
        		if(strncmp(line, format.c_str(), format.length()) == 0){
            			char *token = strtok(line + format.length(), "{}, "); 
            			while(token != NULL){
					char* val =  new char[MAX_LINE_LENGTH]{ '\0' };
                			sscanf(token, "%s", val);
					if(val[0] != '\0'){ value.insert(val); }
                			token = strtok(NULL, "{}, ");
				}
				if(value.size() > 0){ confData[key] = value; USRLOG("group address found"); }
			}
			else{ return -1; }
			return 0;
            }

	public:

		static int initConf(const string& filename){
			FILE *confFile;
       			if((confFile = fopen(filename.c_str(), "r")) == NULL){ SYSLOG; return -1; }

			char line[MAX_LINE_LENGTH] = { '\0' };
    			while(fgets(line, sizeof(line), confFile) != NULL){

				char *comment_pos = NULL;
        			if((comment_pos = strstr(line, "//")) != NULL){ *comment_pos = '\0'; }

				if(initInt(MAC, line) >= 0){ continue; }
				else if(initInt(MAC, line) >= 0){ continue; }
				else if(initInt(MCB, line) >= 0){ continue; }
				else if(initInt(PCOUNT, line) >= 0){ continue; }
				else if(initIntSet(TCP_PORTS, line) >= 0){ continue; }
				else if(initIntSet(UDP_PORTS, line) >= 0){ continue; }
				else if(initStringSet(BGA, line) >= 0){ continue; }
				//else USRLOG("LINE NOT INTERPRET"); 
			}
			return 0;
		}

		template <typename T>
		static T getInfo(string key){ return get<T>(confData[key]); }

		static addr_book a_book;
};

addr_book Conf :: a_book;

map<string, variant<string, int, double, vector<int>, set<int>, set<string>>> Conf::confData = Conf::loadDefaultConfig();


/*
int main(int argc, char** argv){
	cout << MAC << DELIMITER << Conf :: getInfo<int>(MAC) << endl;
	cout << MCB << DELIMITER << Conf :: getInfo<int>(MCB) << endl;
	cout << PCOUNT << DELIMITER << Conf :: getInfo<int>(PCOUNT) << endl;
	set<int> port = Conf :: getInfo<set<int>>(TCP_PORTS);
	set<int>::iterator it = port.begin();
	cout<< TCP_PORTS << DELIMITER << "{";
	while(it != port.end()){
		cout << *it << endl;
		++it;
	}
	cout<< "}" << endl;

	Conf :: initConf(argv[1]);
	cout << MAC << DELIMITER << Conf :: getInfo<int>(MAC) << endl;
	cout << MCB << DELIMITER << Conf :: getInfo<int>(MCB) << endl;
	cout << PCOUNT << DELIMITER << Conf :: getInfo<int>(PCOUNT) << endl;
	set<int> port1 = Conf :: getInfo<set<int>>(TCP_PORTS);
	set<int>::iterator it1 = port1.begin();
	cout<< TCP_PORTS << DELIMITER << "{";
	while(it1 != port1.end()){
		cout << *it1 << endl;
		++it1;
	}
	cout<< "}" << endl;
	return 0;
}
*/
