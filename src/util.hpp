#ifndef _UTIL_HPP_
#define _UTIL_HPP_
#include <iostream>
#include <map>
#include <string>
#include <fstream>
#include <utility>
#define SRV_NAME "Weebtendo"
#define SRV_VER "0.5"
enum nums{CFG=1,NONE,NET,NOTVALID_SOCKET,SOCKET_BIND,LISTEN,SOCK_CONN};
std::map<std::string, std::string> readConfig();

#endif