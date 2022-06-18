#ifndef _SERVER_HPP_
#define _SERVER_HPP_

#include <gccore.h>
#include <network.h>
#include <cstring>
#include <string>
#include <map>
#include <utility>

#include "parser.hpp"
#include "util.hpp"
#include "requestHandler.hpp"

class Server{
	std::string ip, gateway, netmask, root, host;
	uint32_t backlog, port, buffSize, clientLen;
	int sock;
	sockaddr_in client, server;
public:
	Server(std::map<std::string, std::string> config);
	int setup();
	int run();
};
#endif