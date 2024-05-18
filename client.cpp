#include "connect.h"

int main(int argc, char** argv)
{
	if(argc < 2){ Connect cn; cn.initialize(); }
	else { Connect cn(argv[1]); cn.initialize(); }

	getchar();
	return 0;
}	
