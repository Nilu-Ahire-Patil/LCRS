#ifndef SYS_ID_H
#define SYS_ID_H

#include <string> 		// string

#include <uuid/uuid.h>		// uuid

/*-------------------------------------------------------------------------------------------------*/

struct sys_id {
	private:
		uuid_t _id = { 0 };	// unique system id

		// default constructor
		sys_id();

		// friend class access to default constructor
		friend class Conf;

		// for default constructor access
		friend struct packetHeader;

		// for default constructor access
		friend struct chunkHeader;

	public:
		// constructor
		sys_id(const void*);

		// == operator for unordered map
		bool operator==(const sys_id&) const;

		// return system id
		const uuid_t& id() const;

		// return system id
		const std::string str_id() const;

		// check for emptyness
		int isEmpty();

};

/*-------------------------------------------------------------------------------------------------*/

struct sys_id_hash {

	// hash function for hash table
	std::size_t operator()(const sys_id& s_id) const;
};

/*-------------------------------------------------------------------------------------------------*/
/*
#ifndef SYS_ID_IMPL_H
#include "../src/SysId.cpp"
#endif
*/
#endif

/*-------------------------------------------------------------------------------------------------*/
