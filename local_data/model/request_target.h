#ifndef URL_H
#define URL_H

#include <string>
#include <map>


struct request_target {
	std::string target;
	std::map<std::string, std::string> params;
	std::string path;

	explicit request_target(const std::string& s);

private:
	void parse();
};



#endif //URL_H
