#ifndef SYS_ID_IMPL_H
#define SYS_ID_IMPL_H

#include "SysId.h"		// interface

#include <string>		// string
#include <cstring>		// strncmp
				// memcpy
#include <uuid/uuid.h> 		// UUID

/*-------------------------------------------------------------------------------------------------*/

// default constructor
sys_id::sys_id(){}

// constructor 
sys_id::sys_id(const void* buffer){ std::memcpy(_id, buffer, sizeof(_id)); }

// == operator for unordered map
bool sys_id::operator==(const sys_id& s_id) const { return uuid_compare(_id, s_id._id) == 0; }

// return system id
const uuid_t& sys_id::id() const { return this->_id; }

/*-------------------------------------------------------------------------------------------------*/

// hash function for hash map
std::size_t sys_id_hash::operator()(const sys_id& s_id) const {
	// Use a combination of std::hash to hash each byte of the UUID
    	const uuid_t& id = s_id.id();
    	std::size_t hash = 0;
	std::size_t i = 0;
        while(i < sizeof(uuid_t)){
        	hash ^= std::hash<unsigned char>()(id[i]) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
		i++;
    	}
    	return hash;
}

/*-------------------------------------------------------------------------------------------------*/
#endif
