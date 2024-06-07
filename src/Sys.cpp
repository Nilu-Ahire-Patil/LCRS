#ifndef SYS_IMPL_H
#define SYS_IMPL_H

#include "Sys.h"		// Interface

#include <ctime>		// time
				// localtime
				// time_t
				// strftime
#include <cerrno>		// errno
#include <cstring>		// string
				// strerror
#include <iostream>		// fprintf
				// stderr

/*-------------------------------------------------------------------------------------------------*/

// record system error along with user provided message in standerd srror file
void Sys::log(const char* fileName, int lineNo, const char* logType, const std::string& msg) {
	// current time stamp
	time_t now = std::time(NULL);
	char timestamp[64];
	strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", std::localtime(&now));

	// system log line format
	std::string line = "[" + std::string(timestamp) + "]";
       	line += "[" + std::string(logType) + "]";
      	line += ":" + std::string(fileName);
       	line += ":" + std::to_string(lineNo);
	line += ":" + std::string(msg);

	// check for system error
	if(errno){ 
		line += ":" + std::string(strerror(errno)); 
		errno = 0;
	}

	// print system log in standeard error file
	fprintf(stderr, "%s\n", line.c_str());
}

/*-------------------------------------------------------------------------------------------------*/
#endif

/*
void Sys :: log(const string& msg) {
	time_t now = time(NULL);
	char timestamp[64];
	strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", localtime(&now));
 	fprintf(stderr, "[%s] [Info] :%s\n", timestamp, msg.c_str());
}
*/
/*-------------------------------------------------------------------------------------------------*/
