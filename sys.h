#pragma once

#include <ctime>
#include <cerrno>
#include <cstring>
#include <iostream>

//auto generated system log for error and warnings 
#define SYSLOG Sys :: log(__FILE__, __LINE__)

//user generated system log for warnings and display information
#define USRLOG(msg) Sys :: log(__FILE__, __LINE__, msg)							

//stop program execution completely
#define STOP do{\
		cout<<__FILE__ << " : " << __LINE__ << ": STOP SIGNAL TRIGGER" << endl;\
       		exit(EXIT_FAILURE);\
	}while(0)

using namespace std;

class Sys {
	public:
		//system log with file name and line number
		//prints system error
		static void log(const char*, int);

		//system log with file name, line number and message string
		//prints user provided string
		static void log(const char*, int, const string&);
};

void Sys :: log(const char* fileName, int line) {
	//Current time stamp
	time_t now = time(NULL);
	char timestamp[64];
	strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", localtime(&now));
	
 	fprintf(stderr, "[%s] [Error] %s : %d : %s\n", timestamp, fileName, line, strerror(errno));
}

void Sys :: log(const char* fileName, int line, const string& msg) {
	//Current time stamp
	time_t now = time(NULL);
	char timestamp[64];
	strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", localtime(&now));

 	fprintf(stderr, "[%s] [Info] %s : %d : %s\n", timestamp, fileName, line, msg.c_str());
}

/*
void Sys :: log(const string& msg) {
	time_t now = time(NULL);
	char timestamp[64];
	strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", localtime(&now));
 	fprintf(stderr, "[%s] [Info] :%s\n", timestamp, msg.c_str());
}
*/
