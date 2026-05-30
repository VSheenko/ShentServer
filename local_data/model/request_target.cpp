#include "request_target.h"
#include <sstream>


void request_target::parse() {
	auto query_start = target.find('?');

	if (query_start != std::string::npos) {
		path = target.substr(0, query_start);
		std::string query = target.substr(query_start + 1);

		std::stringstream ss(query);
		std::string item;
		while (std::getline(ss, item, '&')) {
			auto equal_pos = item.find('=');
			if (equal_pos != std::string::npos) {
				std::string key = item.substr(0, equal_pos);
				std::string value = item.substr(equal_pos + 1);
				params[key] = value;
			}
		}
	} else {
		path = target;
	}
}

request_target::request_target(const std::string& s) : target(std::move(s)) { parse(); }
