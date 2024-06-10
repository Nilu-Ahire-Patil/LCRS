#ifndef PROTOCOL_H
#define PROTOCOL_H

#include "N_addr.h"

#include <string>

/*-------------------------------------------------------------------------------------------------*/

class Protocol {
	private:

	public:
		// udp handshake request
		int udpHandshakeRequest();

		// message send
		int tcpMessage(const n_addr&, const std::string&);
};

/*-------------------------------------------------------------------------------------------------*/
/*
#ifndef PROTOCOL_IMPL_H
#include "../src/Protocol.cpp"
#endif
*/
#endif

/*-------------------------------------------------------------------------------------------------*/
