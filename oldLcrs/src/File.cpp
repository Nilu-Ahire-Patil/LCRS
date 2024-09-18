#ifndef FILE_INPL_H
#define FILE_INPL_H

#include "File.h"		// interface
#include "FileInfo.h"		// fileInfo
#include "Protocol.h"		// Protocol
#include "Log.h"		// SYSLOG
#include "Configure.h"
#include "Storage.h"

#include <string>		// string
#include <iostream>
#include <fstream>	
#include <unordered_map>
#include <thread> // For std::this_thread::sleep_for
#include <chrono> // For std::chrono::seconds
/*-------------------------------------------------------------------------------------------------*/

// store file on available network 
// return 0 on success
// return non zero value on error
int file::store(const std::string& path){

	std::ifstream file(path, std::ios::binary);
    
    	if(!file){ 
		SYSLOG(ERROR,"FILE_NOT_FOUND " + path);
		return -1;
	}

	Protocol pt;
	// request for adopter
	pt.reqUdpAdopter();

	// wait for 1 seccond for all adopters to respond
	std::this_thread::sleep_for(std::chrono::seconds(1));

	// create file instance
	fileInfo fInfo(path);

	if(fInfo.path().empty()){
		SYSLOG(ERROR,"FILE_STAT_FAIL " + path);
		return -1;
	}

	metaInfo mInfo(fInfo);

	sys_id s_id = Conf::genSysId(path);

	// shared lock on adopters
	std::shared_lock<std::shared_mutex> lock(Protocol::adopter_bookMutex);

	if(Protocol::adopter_book.size() < 1){
		SYSLOG(ERROR,"NO_ADOPTER_FOUND");
		return -1;
	}

	Storage s;

	unsigned int i = 0;
	std::unordered_map<sys_id, n_addr>::iterator it = Protocol::adopter_book.begin();
    	while(!file.eof() && Protocol::adopter_book.size() > 0){

		char* buffer = new char[mInfo.blockSize()];

        	// Read a chunk of data from the file
        	file.read(buffer, mInfo.blockSize());


		// check this chunk variable scope
	//	mInfo[i] = chunk(s_id, i, file.gcount(), data);

		chunk chnk(s_id, i, file.gcount(), buffer);

		delete[] buffer;
		buffer = nullptr;

		while(Protocol::adopter_book.size() > 0){
			if(it == Protocol::adopter_book.end()){ it = Protocol::adopter_book.begin(); }
			if(pt.reqTcpStore(it->second, chnk) < 0){
				SYSLOG(WARN, "ACCESSING_DEAD_NODE " + it->first.str_id());
				it = Protocol::adopter_book.erase(it);
			} else { s.storeMeta(it->first, chnk.header()); break; }
			it++;
		}

		if(Protocol::adopter_book.size() > 0){
			if(it == Protocol::adopter_book.end()){ it = Protocol::adopter_book.begin(); }
		} else { SYSLOG(ERROR,"NO_ADOPTER_FOUND"); return -1; }

		delete[] buffer;
		i++;
		it++;
	}




/*
		while(pt.reqTcpStore(it->second, chunk(s_id, i, file.gcount(), data)) < 0){ 
			SYSLOG(WARN, "ACCESSING_DEAD_NODE " + it->first.str_id());
			
			it = Protocol::adopter_book.erase(it);

			if(Protocol::adopter_book.size() > 0){
				if(it == Protocol::adopter_book.end()){ it = Protocol::adopter_book.begin(); }
			} else { SYSLOG(ERROR,"NO_ADOPTER_FOUND"); return -1; }
		}
		it++;
		if(Protocol::adopter_book.size() > 0){
			if(it == Protocol::adopter_book.end()){ it = Protocol::adopter_book.begin(); }
		} else { SYSLOG(ERROR,"NO_ADOPTER_FOUND"); return -1; }
		
		// update entry in meta file
		s.storeMeta(it->first, mInfo[i].header());
		
    		i++;    
    	}
*/
    	file.close();

	// create meta file
	// create file chunks of decided size
	// send file along with meta file using network class
	return 0;
}

// check an file on available network
// return 0 on find complete file on network
// and stores file on specified path
// return non zero value on error
int file::getFile(const std::string& path, const std::string& storePath){
	// check that file's meta file on network
	// if found get file from network with help of network class
	// store complete file on provided path
	return 0;
}
	
/*-------------------------------------------------------------------------------------------------*/
#endif
