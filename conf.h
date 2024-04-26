class Conf {

	public:
		static constexpr int max_add_capacity = 1024;	//Max capacity of address list
		static constexpr int max_con_buff = 10;		//Max connection waiting buffer

		static const int portCount = 10;
		static unsigned short ports[portCount];
};

unsigned short Conf :: ports[Conf :: portCount] = { 8000, 8001, 8002,
		       	8003, 8004, 8005, 8006, 8007, 8008, 8009 };

#include <stdio.h>
#include <time.h>
#include <cerrno>
#include <string.h>

class Sys {
	public:
		static void log(const char*, int);
		static void log(const char*);
};

void Sys :: log(const char *funcName, int line) {
	time_t now = time(NULL);
	char timestamp[64];
	strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", localtime(&now));
 	fprintf(stderr, "[%s] [Error] %d : %s : %s\n", timestamp, line, funcName, strerror(errno));
}

void Sys :: log(const char *msg) {
	time_t now = time(NULL);
	char timestamp[64];
	strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", localtime(&now));
 	fprintf(stderr, "[%s] [Info] :%s\n", timestamp, msg);
}
