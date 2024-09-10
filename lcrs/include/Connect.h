#ifndef CONNECT_H
#define CONNECT_H

/*-------------------------------------------------------------------------------------------------*/

class Connect {
	private:
		int soc = -1;		//tcp listening socket

	public:
		int initialize();

		Connect();

		Connect(const char* confFilePath);
};

/*-------------------------------------------------------------------------------------------------*/
/*
#ifndef CONNECT_IMPL_H
#include "../src/Connect.cpp"
#endif
*/
#endif

/*-------------------------------------------------------------------------------------------------*/
