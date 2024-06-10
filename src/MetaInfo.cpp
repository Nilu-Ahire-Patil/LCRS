#ifndef META_INFO_IMPL_H
#define META_INFO_IMPL_H

#include "MetaInfo.h"		// Interface
#include "FileInfo.h"		// fileInfo

#include <string>		// string
//#include <sys/stat.h>		// stat
#include <math.h>		// ceil
#include <stdexcept> 		// out_of_range

/*-------------------------------------------------------------------------------------------------*/

// meta file from temparory folder that contains all meta file chunks
metaInfo::metaInfo(const std::string& path){

	// get all chunks
	// initialse meta file variables
	
	// implement this code and remove this
	throw std::out_of_range("Remaining......metaInfo....19");

	this->_chunk = new chunk[this->blocks()];
}

// set file stat variable 
metaInfo::metaInfo(const fileInfo& fileInfo)
	: _path(fileInfo.path()), _size(fileInfo.size()), _blockSize(fileInfo.blockSize()) {
	this->_chunk = new chunk[this->blocks()];
}

chunk& metaInfo::operator[](int index){ 
	if(index < 0 || index >= blockSize()){ throw std::out_of_range("Index out of range"); }
	return this->_chunk[index];
}

const chunk& metaInfo::operator[](int index) const { 
	if(index < 0 || index >= blockSize()){ throw std::out_of_range("Index out of range"); }
	return this->_chunk[index];
}

// returns file path
const std::string& metaInfo::path() const { return this->_path; }

// returns file size in bytes
off_t metaInfo::size() const { return this->_size; }

// returns file block byte size on disk
blksize_t metaInfo::blockSize() const { return this->_blockSize; }

// returns file block numbers for actual data not on disk space blocks  
blkcnt_t metaInfo::blocks() const { return ceil(size()/blockSize()); }

// returns slack space of last block
off_t metaInfo::slackSpace() const { return off_t(size() % blockSize()); }

/*-------------------------------------------------------------------------------------------------*/
#endif
