#ifndef CONFIG_H
#define CONFIG_H

#include "N_addr.h"		//n_addr
#include "SysId.h"		//sys_id

#include <mutex>		// unique_lock
#include <shared_mutex>		// shared_mutex
#include <variant>		// variant
#include <string>		// string
#include <vector>		// vector
#include <set>			// set
#include <unordered_map>	// unordered_map

/*-------------------------------------------------------------------------------------------------*/
#define DEFAULT_CONFIG_PATH "etc/lcrs.config"

#define MAX_PACKET_SIZE 1024
#define MAX_LINE_LENGTH 100
#define DELIMITER 	" = "
#define NAMESPACE_DNS 	"6ba7b810-9dad-11d1-80b4-00c04fd430c8"


#define LP 		"LISTNING_PORT"
#define SYS_ID		"SYS_ID"

/*-------------------------------------------------------------------------------------------------*/

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
// #define NET_INTERFACES	"network_interfaces"
// #define V_NET_INTERFACES "wlp3s0", "enp0s31f6" 

/*-------------------------------------------------------------------------------------------------*/

class Conf {
	private:
		// key value base structure for storing system variables and can initialise from file
   		static std::unordered_map<std::string, std::variant<std::string, int, double, unsigned short, 
			std::vector<int>, std::vector<std::string>, 
			std::set<int>, std::set<std::string>>> confData;

		// mutex for solve critical section problem
		static std::shared_mutex confDataMutex;

		// load default configuration in confData variable
		static int loadDefaultConfig();

		static sys_id s_id;

		// finds integer value in line and insert in configuration data with provided key
		static int initInt(const std::string&, char*);

		// finds string value in line and insert in configuration data with provided key
		static int initString(const std::string&, char*);

		// finds integer set from line and insert in configuration data with provided key
		static int initIntSet(const std::string&, char*);

		// finds string set from line and insert in configuration data with provided key
		static int initStringSet(const std::string&, char*);

		// finds string vector from line and insert in configuration data with provided key
		static int initStringVector(const std::string& key, char* line);

		// initialise system id
		static int initSysId();

	public:
		// initialise system id
		static const sys_id& getSysId();

		// initialise system with default configurstion
		static int initConf();

		// initialise system with configuration file
		static int initConf(const std::string&);

		// get info by key
		template <typename T>
		static T getInfo(const std::string& key){ 
			// Shared lock for reading
			std::shared_lock<std::shared_mutex> lock(confDataMutex);
			return std::get<T>(confData[key]); 
		}

		// set info by key
		template <typename T>
		static void setInfo(const std::string& key, T value){
			// Exclusive lock for writing
			std::unique_lock<std::shared_mutex> lock(confDataMutex);  
		       	confData[key] = value; 
		}
		
		// address book
		static std::unordered_map<sys_id, n_addr, sys_id_hash> addr_book;

		// mutex for solve critical section problem
		static std::shared_mutex addr_bookMutex;
};

/*-------------------------------------------------------------------------------------------------*/
/*
#ifndef CONFIG_IMPL_H
#include "../src/Configure.cpp"
#endif
*/
#endif

/*-------------------------------------------------------------------------------------------------*/
