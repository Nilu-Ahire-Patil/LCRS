#include <stdio.h>
#include <time.h>
#include <cerrno>
#include <string>
#include <string.h>
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include <set>

#define SYSLOG Sys :: log(__FILE__,__LINE__)

using namespace std;

class Sys {
	public:
		static void log(const char*, int);
		static void log(const string&);
};

void Sys :: log(const char* funcName, int line) {
	time_t now = time(NULL);
	char timestamp[64];
	strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", localtime(&now));
 	fprintf(stderr, "[%s] [Error] %d : %s : %s\n", timestamp, line, funcName, strerror(errno));
}

void Sys :: log(const string& msg) {
	time_t now = time(NULL);
	char timestamp[64];
	strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", localtime(&now));
 	fprintf(stderr, "[%s] [Info] :%s\n", timestamp, msg.c_str());
}

//Key Words
#define MAC "max_add_capacity"
#define MCB "max_con_buff"
#define PCOUNT "port_count"
#define PORTS "ports"

using namespace std;

using json = nlohmann::json;

class Conf {
	public:
		static set<int> prts;

		static json initDefaultConf(){      
			static json defaultSettings;
			defaultSettings[MAC] = 24;
        		defaultSettings[MCB] = 10;
        		defaultSettings[PCOUNT] = 3;
        	//	defaultSettings[PORTS] = { 8001, 8002, 8003};
			return defaultSettings;
		}

		static int loadConf(const string& filename) {
        		ifstream file(filename);
        		if(!file.is_open()){SYSLOG; return -1;}
        		file >> configData;

		 	Sys::log("Confurations Loaded successfully");
			return 0;
		}	

		template<typename T>
		static T getInfo(const string& key){return  configData[key].get<T>();}

		static set<int> initPorts() {
        		set<int> ports;
        		auto it = configData.find(PORTS);
        		if(it != configData.end()){// && it->is_array()){
		 		Sys::log("Confurations Loaded successfully");
				for(const auto& elem : *it){ ports.insert(elem.get<int>());}}
			else{ ports.insert(9001);}
        		return ports;
		}

	private:
    		static json configData;
};

json Conf::configData = Conf :: initDefaultConf();	       
set<int> Conf::prts = Conf :: initPorts();

