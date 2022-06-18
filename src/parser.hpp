#ifndef _PARSER_HPP_
#define _PARSER_HPP_

#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <utility>

#include "util.hpp"
class Parser{
	std::string file, root, host, request;
public:
	Parser(char* reqBuffer, uint16_t buffSize, std::string root, std::string host);
	Parser(std::string root, std::string host);
	std::pair<std::string, std::string> getData();
	void setRequest(char* reqBuffer, uint16_t buffSize);
private:
	std::string getToken(std::string delimiter, uint16_t initialPos = 0);
};
#endif