#ifndef META_INFO_IMPL_H
#define META_INFO_IMPL_H

#include "MetaInfo.h"		// Interface
#include "FileInfo.h"		// fileInfo

#include <string>		// string
//#include <sys/stat.h>		// stat
#include <math.h>		// ceil
#include <stdexcept> 		// out_of_range
#include <cstring>

/*-------------------------------------------------------------------------------------------------*/
// default constructor
chunkHeader::chunkHeader(){}

// initialise chunkHeader data
chunkHeader::chunkHeader(const char* data)
	: _s_id(data), _no((unsigned int) (*(data + sizeof(sys_id)))),
       	_dataSize((size_t) (*(data + sizeof(sys_id) + sizeof(unsigned int)))) {}

chunkHeader::chunkHeader(sys_id s_id, unsigned int number, size_t dataSize)
	: _s_id(s_id), _no(number), _dataSize(dataSize) {}

// return block size
size_t chunkHeader::dataSize() const { return this->_dataSize; }

char* chunkHeader::serialize() const {
	char* data = new char[sizeof(chunkHeader)];
	memcpy(data, &_s_id, sizeof(sys_id));
	memcpy(data + sizeof(sys_id), &_no, sizeof(unsigned int));
	memcpy(data + sizeof(sys_id) + sizeof(unsigned int), &_dataSize, sizeof(size_t));

	return data;
}

const sys_id& chunkHeader::id() const { return this->_s_id; }

unsigned int chunkHeader::no() const { return this->_no; }
/*-------------------------------------------------------------------------------------------------*/
// default constructor
chunk::chunk(){}

// deserialize
// initialise all data
chunk::chunk(const char* data):_header(data) {
	this->_data = new char[this->dataSize()];
	memcpy(this->_data, data + sizeof(chunkHeader), this->dataSize());
}

// create chunk
chunk::chunk(sys_id s_id, int no, size_t dataSize, char* data): _header(s_id, no, dataSize) {
	this->_data = new char[this->dataSize()];
	memcpy(this->_data, data, dataSize);
}

// distructor
chunk::~chunk(){ if(this->_data != nullptr){ delete[] this->_data; this->_data = nullptr; } }

// return chunkHeader
const chunkHeader& chunk::header() const { return this->_header; }

// return block size
size_t chunk::dataSize() const { return this->_header.dataSize(); }

// serialize chunk
char* chunk::serialize() const {
	char* buffer  = new char[sizeof(chunkHeader) + this->dataSize()];
	memcpy(buffer, &_header, sizeof(chunkHeader));
	memcpy(buffer + sizeof(chunkHeader), this->_data, this->dataSize());
	return buffer;
}

const sys_id& chunk::id() const { return this->_header.id(); }

const char* chunk::data() const { return this->_data; }
		
unsigned int chunk::no() const { return this->_header.no(); }
/*-------------------------------------------------------------------------------------------------*/

// meta file from temparory folder that contains all meta file chunks
metaInfo::metaInfo(const std::string& folderPath){

	// get all chunks
	// initialse meta file variables
	
//	this->_chunk = new chunk[this->blocks()];
	// implement this code and remove this
}

// set file stat variable 
metaInfo::metaInfo(const fileInfo& fileInfo)
	: _path(fileInfo.path()), _size(fileInfo.size()), _blockSize(fileInfo.blockSize()) {
//	this->_chunk = new chunk[this->blocks()];
}
/*
chunk& metaInfo::operator[](int index){ 
	if(index < 0 || index >= blockSize()){ throw std::out_of_range("Index out of range"); }
	return this->_chunk[index];
}

const chunk& metaInfo::operator[](int index) const { 
	if(index < 0 || index >= blockSize()){ throw std::out_of_range("Index out of range"); }
	return this->_chunk[index];
}
*/
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
