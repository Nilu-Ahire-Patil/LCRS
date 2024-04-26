#include "connect.h"

int main(int argc, char** argv)
{
	if(argc < 2)	return -1;
	struct in_addr addr;
    	inet_aton(argv[1], &addr);
	Connect cn(addr);
	cn.initialize();

	getchar();
	return 0;
}	
