#include "connect.h"

int main(int argc, char** argv)
{
	if(argc < 2){ Connect cn; cn.initialize(); }
	else { Connect cn(argv[1]); cn.initialize(); }

	std::string ip;
	unsigned short port;
	std::string msg;

	packet pkt;
	Network nt;
	
	while(1){
		std::cout << "ip: ";
		std::cin >> ip;
		std::cout << "port: ";
		std::cin >> port;
		std::cout << "message: ";
		std::cin >> msg;

		pkt.init(packetType::Message,(void*) msg.c_str(), msg.length());

		n_addr addr(ip, port);

		nt.sendTcpPacket(pkt, addr);
		
		pkt.freePacketData();
	}

	getchar();
	return 0;
}	
