#ifndef LOG_H
#define LOG_H

#include "N_addr.h"		// n_addr
#include "PacketHeader.h"	// packetType

#include <string>		// string

/*-------------------------------------------------------------------------------------------------*/

// types of error log and color for them
#define ERROR 	"\033[31mError\033[0m"		// RED
#define INFO 	"\033[32mInfo\033[0m"		// BLUE
#define WARN 	"\033[33mWarning\033[0m"	// YELLO
#define SUCCESS "\033[34mSuccess\033[0m"	// GREEN


#define RECV 	"\033[34mrecv\033[0m"		// GREEN
#define SEND 	"\033[32msend\033[0m"		// BLUE
#define FAIL 	"\033[31msend\033[0m"		// RED

/*-------------------------------------------------------------------------------------------------*/

// macro for system log 
// prints error with filename and line along with time stamp 
#define SYSLOG(logType, msg) \
		Log::log( \
			logType, \
			msg, \
		       	__FILE__, \
		       	__LINE__ \
		)

// macro for packet log 
// prints sender, receiver, packet type with packet size along with time stamp 
#define PKTLOG(sendRecv, sender_sendTo, ptype, size) \
		Log::pktlog( \
			sendRecv, \
			std::string(inet_ntoa(sender_sendTo.sin_addr)) + ":" + \
			std::to_string(ntohs(sender_sendTo.sin_port)), \
			packetTypeToString(ptype), \
			size, \
		       	__FILE__, \
	       		__LINE__  \
		)

// macro for system crash signal
// this macro stop the complete system working
// prints the error with file name and line along with time stamp
#define STOP(logType, msg) \
		do{ \
			SYSLOG(logType, msg); \
		       	exit(EXIT_FAILURE); \
		}while(0);


/*-------------------------------------------------------------------------------------------------*/

class Log {
	public:
		//system log with file name and line number
		//prints system error
		static void log(const char*, const std::string&, const char*, int);

		// prints sender, receiver, packet type with packet size along with time stamp 
		static void pktlog(const char*, const std::string&, 
				std::string, unsigned int, const char*, int);

		// gives current date in YYYYMMDD this format
		static std::string getCurrentDateString();
};

/*-------------------------------------------------------------------------------------------------*/
/*
#ifndef SYS_IMPL_H
#include "../src/Sys.cpp"
#endif
*/
#endif

/*-------------------------------------------------------------------------------------------------*/
