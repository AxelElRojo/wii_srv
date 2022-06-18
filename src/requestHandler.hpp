#ifndef _HANDLER_HPP_
#define _HANDLER_HPP_

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <ctime>
#include <map>
#include <utility>
#include <vector>
#include <cstring>

#include "util.hpp"
class RequestHandler{
	static std::map<std::string, std::string> fileTypes;
	std::string root, host;
	char* file;
	uint32_t filesize, headersize;
	bool includeBody;
public:
	RequestHandler(std::string root, std::string host);
	~RequestHandler();
	std::string handleRequest(std::string type, std::string filename);
	uint32_t getResponseSize();
	uint32_t getHeaderSize();
	char* getResponse();
	bool getIncludeBody();
private:
	bool readFile(std::string filename);
	std::string processRequest(bool includeBody, std::string filename = "");
	std::string generateHeader(uint16_t code, std::string filename = "");
	std::string currentTime();
	std::string getFileExtension(std::string filename);
	std::map<std::string, std::string> readMime();
};
#endif