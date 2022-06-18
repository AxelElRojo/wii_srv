#include "requestHandler.hpp"
std::map<std::string, std::string> RequestHandler::fileTypes;
RequestHandler::RequestHandler(std::string root, std::string host){
	this->root = root;
	this->host = host;
	this->filesize = 0;
	this->fileTypes = this->readMime();
	this->file = nullptr;
}
RequestHandler::~RequestHandler(){
	if(this->file)
		delete[] this->file;
}
std::string RequestHandler::getFileExtension(std::string filename){
	return filename.substr(filename.find_last_of('.') + 1);
}
std::string RequestHandler::handleRequest(std::string type, std::string filename){
	if(type != "GET" && type != "HEAD")
		return generateHeader(501);
	if(filename[filename.length() - 1] == '/')
		filename += "index.html";
	if(filename[0] == '/')
		filename.erase(0, 1);
	filename = this->root + filename;
	std::cout << "Requested " << filename << '\n';
	if(!this->readFile(filename))
		return generateHeader(404);
	if(type == "GET"){
		std::cout << "GET " << filename << '\n';
		return processRequest(true, filename);
	}else if(type == "HEAD")
		return processRequest(false, filename);
	else if(type == "POST" || type == "DELETE" || type == "PATCH" || type == "PUT" ||
			type == "CONNECT" || type == "OPTIONS" || type == "TRACE")
		return generateHeader(501);
	else if(type == "BREW")
		return generateHeader(418);
	else
		return generateHeader(400);
}
std::string RequestHandler::generateHeader(uint16_t code, std::string filename){
	std::stringstream header;
	header << "HTTP/1.0 ";
	switch(code){
		case 200: // OK
			header << "200 OK\r\n";
		break;
		case 400: // Bad Request
			header << "400 Bad Request\r\n";
		break;
		case 404: // NOT FOUND
			header << "404 Not Found\r\n";
		break;
		case 418: // I'm a teapot
			header << "418 I'm a teapot\r\n";
		break;
		case 501: // NOT IMPLEMENTED
			header << "501 Not Implemented\r\n";
		break;
		case 505: // VER NOT SUPPORTED
			header << "505 HTTP Version Not Supported\r\n";
		break;
	}
	header << "Date: " << this->currentTime() << "\r\n"
		<< "Server: " << SRV_NAME << '/' << SRV_VER << " (Nintendo Wii)";
	if(this->filesize != 0)
		header << "\r\nContent-Length: " << this->filesize;
	if(filename != "")
		header << "\r\nContent-Type: " << fileTypes[ getFileExtension(filename) ];
	header << "\r\n\r\n";
	return header.str();
}
bool RequestHandler::readFile(std::string filename){
	std::ifstream fileReader;
	std::stringstream buffer;
	if(this->file){
		delete[] this->file;
		this->file = nullptr;
	}
	fileReader.open(filename, std::ios::binary);
	if(!fileReader.is_open())
		return false;
	fileReader.seekg(0, std::ios::end);
	this->filesize = fileReader.tellg();
	fileReader.seekg(0, std::ios::beg);
	this->file = new char[this->filesize];
	fileReader.read(this->file, this->filesize);
	return true;
}
std::string RequestHandler::processRequest(bool includeBody, std::string filename){
	std::string response = generateHeader(200, filename);
	this->includeBody = includeBody;
	return response;
}
std::string RequestHandler::currentTime(){
	time_t now;
	struct tm* timeinfo;
	char buffer[32];
	time(&now);
	timeinfo = gmtime(&now);
	strftime(buffer, sizeof(buffer), "%a, %d %b %Y %H:%M:%S GMT", timeinfo);
	return buffer;
}
std::map<std::string, std::string> RequestHandler::readMime(){
	std::ifstream reader("/web/file.types");
	std::map<std::string, std::string> types;
	std::string key, type;
	if(reader.is_open()){
		while(reader >> key >> type)
			if(key != "#")
				types[key] = type;
		std::cout << "MIME read\n";
	}else{
		types["html"] = "text/html";
		types["css"] = "text/css";
		types["js"] = "text/javascript";
		types["json"] = "application/json";
		types["jpg"] = "image/jpeg";
		types["ico"] = "image/x-icon";
		types["png"] = "image/png";
		std::cout << "Can't read MIME\n";
	}
	return types;
}
uint32_t RequestHandler::getResponseSize(){
	return this->filesize;
}
uint32_t RequestHandler::getHeaderSize(){
	return this->headersize;
}
char* RequestHandler::getResponse(){
	return this->file;
}
bool RequestHandler::getIncludeBody(){
	return this->includeBody;
}