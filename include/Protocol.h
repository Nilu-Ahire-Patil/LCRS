#ifndef PROTOCOL_H
#define PROTOCOL_H

#include "N_addr.h"		// n_addr
#include "SysId.h"		// sys_id
#include "MetaInfo.h"		// chunk

#include <string>
#include <unordered_map>
#include <shared_mutex>

/*-------------------------------------------------------------------------------------------------*/

class Protocol {
	private:
		// adopter book for store file chunks
		static std::unordered_map<sys_id, n_addr, sys_id_hash> adopter_book;

		// mux for solve critical section problem
		static std::shared_mutex adopter_bookMutex;

		// friend class for access adopter_book
		friend class Network;

	public:
		// udp handshake request
		int reqUdpHandshake();

		// reply for request udphandshake using tcp hashdshake
		int repTcpHandshake(const n_addr);

		// request for adopter using udp
		int reqUdpAdopter();

		// reply for request adopter using tcp
		int repTcpAdopter(const n_addr);

		// request for store chunk using tcp
		int reqTcpStore(const n_addr, const chunk&);

		// reply for request store using tcp
		int repTcpStore(const n_addr, const chunk&);

		// text message send
		int repTcpMessage(const n_addr&, const std::string&);
};

/*-------------------------------------------------------------------------------------------------*/
/*
#ifndef PROTOCOL_IMPL_H
#include "../src/Protocol.cpp"
#endif
*/
#endif

/*-------------------------------------------------------------------------------------------------*/
