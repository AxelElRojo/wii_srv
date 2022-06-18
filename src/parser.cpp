#include "parser.hpp"
Parser::Parser(char* reqBuffer, uint16_t buffSize, std::string root, std::string host){
	this->request = reqBuffer;
	this->root = root;
	this->host = host;
}
Parser::Parser(std::string root, std::string host){
	this->root = root;
	this->host = host;
}
void Parser::setRequest(char* reqBuffer, uint16_t buffSize){
	this->request = reqBuffer;
}
std::string Parser::getToken(std::string delimiter, uint16_t initialPos){
	return this->request.substr(initialPos, this->request.find(delimiter, initialPos) - initialPos );
}
std::pair<std::string, std::string> Parser::getData(){
	std::pair<std::string, std::string> data;
	data.first = this->getToken(" ");
	data.second = this->getToken(" ", data.first.length() + 1);
	return data;
}