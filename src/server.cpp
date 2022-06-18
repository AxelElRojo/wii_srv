#include "server.hpp"
Server::Server(std::map<std::string, std::string> config){
	char ip[16], netmask[16], gateway[16];
	std::cout << "Creating server\n";
	if(if_config(ip, netmask, gateway, TRUE, 20) >= 0){
		this->ip = ip;
		this->netmask = netmask;
		this->gateway = gateway;
		this->backlog = std::stoull(config["queue"]);
		this->port = std::stoull(config["port"]);
		this->buffSize = std::stoull(config["buffer"]);
		this->root = config["root"];
		this->host = config["host"];
		this->clientLen = sizeof(this->client);
		std::cout << "Listening on " << ip << ':' << port << '\n';
	}else
		std::cout << "Can't configure the network\n";
}
int Server::setup(){
	std::cout << "Setting up server\n";
	int ret;
	this->sock = net_socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
	if(this->sock == INVALID_SOCKET){
		std::cout << "Invalid socket\n";
		return NOTVALID_SOCKET;
	}
	std::memset(&(this->server), 0, sizeof(this->server));
	std::memset(&(this->client), 0, sizeof(this->client));
	server.sin_family = AF_INET;
	server.sin_port = htons(this->port);
	server.sin_addr.s_addr = INADDR_ANY;
	ret = net_bind(this->sock, (sockaddr*)&(this->server), sizeof(this->server));
	if(ret){
		std::cout << ret << " error binding socket\n";
		return SOCKET_BIND;
	}
	ret = net_listen(this->sock, this->backlog);
	if(ret){
		std::cout << "Error listening";
		return LISTEN;
	}
	return 0;
}
int Server::run(){
	std::cout << "Server running\n";
	int clientSock;
	char buffer[this->buffSize];
	std::string header;
	Parser* parser = new Parser(this->root, this->host);
	RequestHandler* handler = new RequestHandler(this->root, this->host);
	std::pair<std::string, std::string> data;
	int64_t responseSize;
	this->clientLen = sizeof(client);
	while(true){
		clientSock = net_accept(this->sock, (sockaddr*)&(this->client), &(this->clientLen));
		if(clientSock < 0){
			std::cout << "Error accepting socket " << clientSock << '\n';
			return SOCK_CONN;
		}
		std::memset(buffer, 0, this->buffSize);
		net_recv(clientSock, buffer, this->buffSize, 0);

		parser->setRequest(buffer, this->buffSize);
		data = parser->getData();

		header = handler->handleRequest(data.first, data.second);
		responseSize = handler->getResponseSize();
		net_send(clientSock, header.c_str(), header.length(), 0);
		net_send(clientSock, handler->getResponse(), responseSize, 0);
		///*
		for(int64_t totalSent=0, sent; totalSent < responseSize; totalSent += sent){
			sent = net_send(clientSock, handler->getResponse() + totalSent, responseSize - totalSent, 0);
			if(sent == SOCKET_ERROR || sent < 0){
				std::cout << "Send error";
				break;
			}
		}
		//*/
		FILE* ptr = fdopen(clientSock, "w+");
		fflush(ptr);
		net_close(clientSock);
	}
	delete parser;
	delete handler;
}