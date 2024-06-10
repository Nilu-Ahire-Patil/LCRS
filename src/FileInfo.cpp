#ifndef FILE_INFO_IMPL_H
#define FILE_INFO_IMPL_H

#include "FileInfo.h"		// interface
#include "Sys.h"		// SYSLOG, STOP

#include <string>		// string
//#include <sys/stat.h>		// stat
#include <math.h>		// ceil

/*-------------------------------------------------------------------------------------------------*/

// set file stat variable with provided file path
fileInfo::fileInfo(const std::string& path){
	if(stat(path.c_str(), &this->_fileStat) < 0){ SYSLOG(INFO, "unable to get file stat"); }
	// check for file type accept normal file
	// don't access folder
	// also check how it works with zip and other type of files
	this->_path = path;
}

// returns file path
std::string fileInfo::path() const { return this->_path; }

// returns file size in bytes
off_t fileInfo::size() const { return this->_fileStat.st_size; }

// returns file block byte size on disk
blksize_t fileInfo::blockSize() const { return this->_fileStat.st_blksize; }

// returns file block numbers for actual data not on disk space blocks  
blkcnt_t fileInfo::blocks() const { return blksize_t(ceil(size() / blockSize())); }

// returns slack space of last block
off_t fileInfo::slackSpace() const { return off_t(size() % blockSize()); }

/*-------------------------------------------------------------------------------------------------*/
#endif
