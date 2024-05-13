#include "connect.h"

int main(int argc, char** argv)
{
	if(argc < 2){ cout << "provde Config File name" << endl; return 1; }

/*
	in_addr i;
	n_addr n1 = n_addr(i, 123);
	Conf ::	a_book.add(n1);

	n_addr n2 = n_addr(i, 124);
	Conf ::	a_book.add(n2);

	n_addr n3 = n_addr(i, 123);
	Conf ::	a_book.add(n3);
	cout << "size : "<< Conf :: a_book.size() << endl;

*/
	

	Connect cn(argv[1]);
	cn.initialize();

	getchar();
	return 0;
}	
