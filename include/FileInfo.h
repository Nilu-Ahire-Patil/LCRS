#ifndef FILE_INFO_H
#define FILE_INFO_H

#include <string>		// string
#include <sys/stat.h>		// stat

/*-------------------------------------------------------------------------------------------------*/
struct fileInfo {
	private:
		std::string _path;		// file path in local machine
		struct stat _fileStat;		// file information

	public:
		// set file stat variable with provided file path
		fileInfo(const std::string& path);

		// returns file path
		std::string path() const;

		// returns file size in bytes
		off_t size() const;

		// returns file block byte size on disk
		blksize_t blockSize() const;

		// returns file block(512 bytes per block) numbers on actual disk 
		blkcnt_t blocks() const;

		// retuens slack space of last block
		off_t slackSpace() const;
};

/*-------------------------------------------------------------------------------------------------*/
/*
#ifndef FILE_INFO_IMPL_H
#include "../src/FileInfo.cpp"
#endif
*/
#endif

/*-------------------------------------------------------------------------------------------------*/
