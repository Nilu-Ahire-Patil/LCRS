using namespace std;

class Network {

	public:
		int getTcpSocket();

		int bindTcp(int);
		int bindTcp(int, unsigned short*);

		int listenTcp(int);
		int acceptTcp(int);

		int connectTcp(int, in_addr);
		int connectTcp(int, in_addr, unsigned short int);
};

int Network :: getTcpSocket(){

	int soc = -1;
	if((soc = socket(AF_INET, SOCK_STREAM, 0)) < 0) { Sys :: log(__func__,__LINE__); return -1;}

	int optval = 1;
	if(setsockopt(soc, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0) { Sys :: log(__func__,__LINE__);}

	return soc;
}

int Network :: bindTcp(int soc, unsigned short* listning_port){
	if(soc < 0) return soc;

	int loop = 0;
	while(loop < Conf :: portCount){
		sockaddr_in add;
		memset(&add, 0, sizeof(sockaddr_in));
		add.sin_family = AF_INET;
		add.sin_port = htons(Conf :: ports[loop]);
		add.sin_addr.s_addr = INADDR_ANY;

		if(bind(soc, (sockaddr*) &add, sizeof(add)) < 0) { Sys :: log(__func__,__LINE__);}
		else {	
			*listning_port = Conf :: ports[loop];
			Sys :: log("port configure");
			cout << "port "<< Conf :: ports[loop]<< endl;
			loop = Conf :: portCount;	//Exit condition executes when all ok
		}

		loop++;
	}

	return soc;
}

int Network :: bindTcp(int soc){
	if(soc < 0) return soc;

	int loop = 0;
	while(loop < Conf :: portCount){
		sockaddr_in add;
		memset(&add, 0, sizeof(sockaddr_in));
		add.sin_family = AF_INET;
		add.sin_port = htons(Conf :: ports[loop]);
		add.sin_addr.s_addr = INADDR_ANY;

		if(bind(soc, (sockaddr*) &add, sizeof(add)) < 0) { Sys :: log(__func__,__LINE__);}
		else {	Sys :: log("port configure");
			cout << "port "<< Conf :: ports[loop]<< endl;
			loop = Conf :: portCount;	//Exit condition executes when all ok
		}

		loop++;
	}

	return soc;
}

int Network :: listenTcp(int soc){
	if(soc < 0) return soc;

	if(listen(soc, Conf :: max_con_buff) < 0) { Sys :: log(__func__,__LINE__);}

	return soc;
}

int Network :: acceptTcp(int soc){
	if(soc < 0) return soc;

	int csoc = -1;
	sockaddr_in c_addr;
	socklen_t c_addr_len = sizeof(c_addr);

	if((csoc = accept(soc, (struct sockaddr*) &c_addr, &c_addr_len)) < 0) { Sys :: log(__func__,__LINE__);}

	return csoc;
}

int Network :: connectTcp(int soc, in_addr ip){
	if(soc < 0) return soc;

	int loop = 0;
	while(loop < Conf :: portCount){
		if(connectTcp(soc, ip, Conf :: ports[loop]) < 0) { Sys :: log(__func__,__LINE__);}
		else{ loop = Conf :: portCount;}
		loop++;
	}

	return soc;
}

int Network :: connectTcp(int soc, in_addr ip, unsigned short int port){
	if(soc < 0) return soc;

	struct sockaddr_in s_addr;
	socklen_t s_addr_len = sizeof(s_addr);

	memset(&s_addr, 0, sizeof(sockaddr_in));
	s_addr.sin_family = AF_INET;
	s_addr.sin_port = htons(port);
	s_addr.sin_addr = ip;

	if(connect(soc,(struct sockaddr*) &s_addr, s_addr_len) < 0) { Sys :: log(__func__,__LINE__);}

	return soc;
}
