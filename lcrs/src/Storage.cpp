#ifndef STORAGE_IMPL_H
#define STORAGE_IMPL_H

#include "Storage.h"		// interface
#include "SysId.h"
#include "MetaInfo.h"
#include "Log.h"
#include "Configure.h"

#include <iostream>
#include <fstream>
#include <string>

/*-------------------------------------------------------------------------------------------------*/
// append chunk info in their apropiet file
int Storage::storeMeta(const sys_id& s_id, const chunkHeader& chnk){

	// Open the file in append mode and create it if it does not exist
	std::string filename = Conf::getInfo<std::string>(DDD) + chnk.id().str_id()+ ".meta";
    	std::ofstream file(filename, std::ios::app | std::ios::out);

    	if(!file){
		SYSLOG(ERROR, "FILE_NOT_FOUND " + filename);
		return -1;
	}
	// storing id of adopter
	std::string line = std::to_string(chnk.no()) + " : " + std::to_string(chnk.dataSize()) + " --> " + s_id.str_id() + "\n";

    	file.write(line.c_str(), line.length());

	SYSLOG(INFO, filename + " " + std::to_string(line.length()) + " FILE_APPEND");

    	// Close the file
    	file.close();

	return 0;

}

// store chunk in appropiate file
int Storage::storeChunk(const chunk& chnk){
	// Open the file in append mode and create it if it does not exist
	std::string filename = Conf::getInfo<std::string>(DDD) + chnk.id().str_id() + "." + std::to_string(chnk.no()) + ".chunk";
    	std::ofstream file(filename, std::ios::app | std::ios::out);

    	if(!file){
		SYSLOG(ERROR, "FILE_NOT_FOUND " + filename);
		return -1;
	}
	// Write the char* data to the file
    	file.write(chnk.data(), chnk.dataSize());

	SYSLOG(INFO, filename + " " + std::to_string(chnk.dataSize()) + " FILE_WRITE");

    	// Close the file
    	file.close();

	return 0;
}




/*-------------------------------------------------------------------------------------------------*/
#endif

/*-------------------------------------------------------------------------------------------------*/
