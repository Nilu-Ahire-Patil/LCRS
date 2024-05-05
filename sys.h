#pragma once

#include <ctime>
#include <cerrno>
#include <cstring>
#include <iostream>

#define SYSLOG Sys :: log(__FILE__,__LINE__)	//auto generated system log for error and warnings 
#define USRLOG(msg) Sys :: log(msg)		//user generated system log for warnings and display information

using namespace std;

class Sys {
	public:
		static void log(const char*, int);
		static void log(const string&);
};

void Sys :: log(const char* funcName, int line) {
	time_t now = time(NULL);
	char timestamp[64];
	strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", localtime(&now));
 	fprintf(stderr, "[%s] [Error] %d : %s : %s\n", timestamp, line, funcName, strerror(errno));
}

void Sys :: log(const string& msg) {
	time_t now = time(NULL);
	char timestamp[64];
	strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", localtime(&now));
 	fprintf(stderr, "[%s] [Info] :%s\n", timestamp, msg.c_str());
}
