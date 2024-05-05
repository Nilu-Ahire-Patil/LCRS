#pragma once

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
#define PORTS 		"port"

//default Values
#define V_MAC 		24
#define V_MCB 		10 
#define V_PCOUNT	3 
#define V_PORTS 	9090, 9091, 9092

using namespace std;

class Conf {
	private:

   		static map<string, variant<string, int, double, vector<int>, set<int>>> confData;

		static map<string, variant<string, int, double, vector<int>, set<int>>> loadDefaultConfig(){
   			static map<string, variant<string, int, double, vector<int>, set<int>>> confData;
	    		confData[MAC] = V_MAC;
			confData[MCB] = V_MCB;
			confData[PCOUNT] = V_PCOUNT;
			confData[PORTS] = set<int>{ V_PORTS };
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
            			char *token = strtok(line+format.length(), "{, "); 
            			while(token != NULL){
					int val = -1;
                			sscanf(token, "%d", &val);
					if(val > 0){ value.insert(val); }
                			token = strtok(NULL, "{, ");
				}
				if(value.size() > 0){ confData[key] = value; }

			}
			else{ return -1; }
			return 0;
            }

	public:


		static int initConf(const string& filename){
			FILE *confFile;
       			if((confFile = fopen(filename.c_str(), "r")) == NULL){ SYSLOG; return -1; }

			char line[MAX_LINE_LENGTH];
    			while(fgets(line, sizeof(line), confFile) != NULL){

				char *comment_pos = NULL;
        			if((comment_pos = strstr(line, "//")) != NULL){ *comment_pos = '\0'; }

				if(initInt(MAC, line) >= 0){ continue; }
				else if(initInt(MAC, line) >= 0){ continue; }
				else if(initInt(MCB, line) >= 0){ continue; }
				else if(initInt(PCOUNT, line) >= 0){ continue; }
				else if(initIntSet(PORTS, line) >= 0){ continue; }
				//else USRLOG("LINE NOT INTERPRET"); 
			}
			return 0;
		}

		template <typename T>
		static T getInfo(string key){
			return get<T>(confData[key]);
		}
};

map<string, variant<string, int, double, vector<int>, set<int>>> Conf::confData = Conf::loadDefaultConfig();


/*
int main(int argc, char** argv){
	cout << MAC << DELIMITER << Conf :: getInfo<int>(MAC) << endl;
	cout << MCB << DELIMITER << Conf :: getInfo<int>(MCB) << endl;
	cout << PCOUNT << DELIMITER << Conf :: getInfo<int>(PCOUNT) << endl;
	set<int> port = Conf :: getInfo<set<int>>(PORTS);
	set<int>::iterator it = port.begin();
	cout<< PORTS << DELIMITER << "{";
	while(it != port.end()){
		cout << *it << endl;
		++it;
	}
	cout<< "}" << endl;

	Conf :: initConf(argv[1]);
	cout << MAC << DELIMITER << Conf :: getInfo<int>(MAC) << endl;
	cout << MCB << DELIMITER << Conf :: getInfo<int>(MCB) << endl;
	cout << PCOUNT << DELIMITER << Conf :: getInfo<int>(PCOUNT) << endl;
	set<int> port1 = Conf :: getInfo<set<int>>(PORTS);
	set<int>::iterator it1 = port1.begin();
	cout<< PORTS << DELIMITER << "{";
	while(it1 != port1.end()){
		cout << *it1 << endl;
		++it1;
	}
	cout<< "}" << endl;
	return 0;
}
*/
