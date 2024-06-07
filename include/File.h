#ifndef FILE_H
#define FILE_H

#include <string>		//string

/*-------------------------------------------------------------------------------------------------*/

class file {
	private:
		
	public:
		// store file on available network 
		// return 0 on success
		// return non zero value on error
		int store(const std::string&);

		// check an file on available network
		// return 0 on find complete file on network
		// and stores file on specified path
		// return non zero value on error
		int getFile(const std::string&, const std::string&);
};
	
/*-------------------------------------------------------------------------------------------------*/
/*
#ifndef FILE_IMPL_H
#include "../src/File.cpp"
#endif
*/
#endif

/*-------------------------------------------------------------------------------------------------*/

