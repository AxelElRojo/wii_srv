#include "util.hpp"
std::map<std::string, std::string> readConfig(){
	std::ifstream reader("/web/conf.cfg");
	std::map<std::string, std::string> config;
	std::string key, val;
	if(reader.is_open()){
		while(reader >> key >> val)
			if(key != "#")
				config[key] = val;
		std::cout << "Config read\n";
	}else{
		std::cout << "Can't read config\n";
		config["port"] = "80";
		config["queue"] = "5";
		config["buffer"] = "1024";
		config["host"] = "wii.wii";
		config["root"] = "/web/public/";
	}
	return config;
}