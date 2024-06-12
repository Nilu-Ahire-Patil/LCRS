#ifndef META_INFO_H
#define META_INFO_H

#include "FileInfo.h"		// fileInfo
#include "SysId.h"		// sys_id

#include <string>		// string
#include <sys/stat.h>		// stat

struct chunkHeader {
	private:
		sys_id _s_id;
		unsigned int _no;
		
	public: 
		// default constructor
		chunkHeader();

		// serialise chunkHeader
		chunkHeader(const char*);

		// serialise chunkHeader
		char* serialize() const;
};

struct chunk {
	private:
		struct chunkHeader _id;
		size_t _dataSize;
	public:

		chunk();

		// deserialise chuk
		chunk(const char*);

		// returns chunkHeader
		const chunkHeader& id() const; 

		// return block size
		size_t dataSize() const;

		// serialise chunkHeader
		char* serialize() const;
		// chunk hash
		// chunk size
		// chunk adoptive node id
		// ip
		// port
		// status flag sent receive replicated etc
};



/*-------------------------------------------------------------------------------------------------*/
struct metaInfo {
	private:
		std::string _path;	// file path of orignal file in local machine
		off_t _size;		// complete size of orignl file in local machine 
		blksize_t _blockSize;	// block size in bytes
					// 0 for variable length blocks
			
		chunk* _chunk;		// array of chunks;
		

	public:
		// set file stat variable with provided metafile path file path
		metaInfo(const std::string& path);

		// set file stat variable 
		metaInfo(const fileInfo& fileInfo);

		// distrector for free chunk*
		~metaInfo(){ delete[] _chunk; _chunk = nullptr; }

		// access and modify chunk data
		chunk& operator[](int);

		// access chunk data
		const chunk& operator[](int) const;

		// returns file path
		const std::string& path() const;

		// returns file size in bytes
		off_t size() const;

		// returns file block byte size on disk
		blksize_t blockSize() const;

		// returns file block numbers for actual data not on disk space blocks  
		blkcnt_t blocks() const;

		// retuens slack space of last block
		off_t slackSpace() const;
};

/*-------------------------------------------------------------------------------------------------*/
/*
#ifndef META_INFO_IMPL_H
#include "../src/MetaInfo.cpp"
#endif
*/
#endif

/*-------------------------------------------------------------------------------------------------*/
