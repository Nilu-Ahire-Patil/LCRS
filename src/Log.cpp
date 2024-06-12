#ifndef LOG_IMPL_H
#define LOG_IMPL_H

#include "Log.h"		// interface

#include <ctime>		// time
				// localtime
				// time_t
				// strftime
#include <cerrno>		// errno
#include <cstring>		// string
				// strerror
#include <iostream>		// fprintf
				// stderr
#include <iomanip>
#include <sstream>

/*-------------------------------------------------------------------------------------------------*/

// record system error along with user provided message in standerd srror file
void Log::log(const char* logType, const std::string& msg, const char* fileName, int lineNo){
	// current time stamp
	time_t now = std::time(NULL);
	char timestamp[64];
	strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", std::localtime(&now));

	// system log line format
	std::string line = "[" + std::string(timestamp) + "]";
       	line += "[" + std::string(logType) + "]";
	line += ":" + std::string(msg);
	line += ":";
	// check for system error
	if(errno){ 
		line += std::string(strerror(errno)); 
		errno = 0;
	}

	// file name and line number for debug
      	line += ":" + std::string(fileName);
       	line += ":" + std::to_string(lineNo);

	// print system log in standeard error file
	fprintf(stderr, "%s\n", line.c_str());
	fflush(stderr);
}

void Log::pktlog(const char* sendRecv, const std::string& sender_sendTo,
		std::string pType, unsigned int size, const char* fileName, int lineNo){

	// current time stamp
	time_t now = std::time(NULL);
	char timestamp[64];
	strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", std::localtime(&now));

	// system log line format
	std::string line = "[" + std::string(timestamp) + "]";
       	line += "[" + std::string(sendRecv) + "]";
	line += "[" + sender_sendTo + "]";
	line += "[" + pType + "]";
	line += ":" + std::to_string(size);

	line += ":";
	// check for system error
	if(errno){ 
		line += std::string(strerror(errno)); 
		errno = 0;
	}

	// file name and line number for debug
      	line += ":" + std::string(fileName);
       	line += ":" + std::to_string(lineNo);

	// print system log in standeard error file
	fprintf(stderr, "%s\n", line.c_str());
	fflush(stderr);
}

std::string Log::getCurrentDateString() {
    // Get current time
    std::time_t t = std::time(nullptr);

    // Convert to local time
    std::tm* localTime = std::localtime(&t);

    // Create a string stream to format the date
    std::ostringstream oss;
    oss << std::put_time(localTime, "%Y-%m-%d");

    // Return the formatted string
    return oss.str();
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
