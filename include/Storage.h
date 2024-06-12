#ifndef STORAGE_H
#define STORAGE_H

#include "SysId.h"		// sys_id
#include "MetaInfo.h"		// chunk

/*-------------------------------------------------------------------------------------------------*/
class Storage {
	private:

	public:
		// append chunk info in their apropiet file
		int storeMeta(const sys_id&,const chunkHeader&);

		// store chunk in appropiate file
		int storeChunk(const chunk&);
};

/*-------------------------------------------------------------------------------------------------*/
/*
#ifndef STORAGE_IMPL_H
#include "../src/Storage.cpp"
#endif
*/
#endif

/*-------------------------------------------------------------------------------------------------*/
