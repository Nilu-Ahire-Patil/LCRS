#ifndef FILE_INPL_H
#define FILE_INPL_H

#include "File.h"		// interface
#include "FileInfo.h"		// fileInfo

#include <string>		// string
	
/*-------------------------------------------------------------------------------------------------*/

// store file on available network 
// return 0 on success
// return non zero value on error
int file::store(const std::string& path){
	// create file instance
	// check permeations of file
	// decide file chunk size
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
