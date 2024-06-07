#ifndef SYS_H
#define SYS_H

#include <string>		// string

/*-------------------------------------------------------------------------------------------------*/

// types of error log and color for them
#define ERROR 	"\033[31mError\033[0m"		// RED
#define INFO 	"\033[32mInfo\033[0m"		// BLUE
#define WARN 	"\033[33mWarning\033[0m"	// YELLO
#define SUCCESS "\033[34mSuccess\033[0m"	// GREEN

/*-------------------------------------------------------------------------------------------------*/

// macro for system log 
// prints error with filename and line along with time stamp 
#define SYSLOG(logType, msg) Sys::log(__FILE__, __LINE__, logType, msg)

// macro for system crash signal
// this macro stop the complete system working
// prints the error with file name and line along with time stamp
#define STOP(logType, msg) SYSLOG(logType, msg); exit(EXIT_FAILURE)

/*-------------------------------------------------------------------------------------------------*/

class Sys {
	public:
		//system log with file name and line number
		//prints system error
		static void log(const char*, int, const char*, const std::string&);

};

/*-------------------------------------------------------------------------------------------------*/
/*
#ifndef SYS_IMPL_H
#include "../src/Sys.cpp"
#endif
*/
#endif

/*-------------------------------------------------------------------------------------------------*/
