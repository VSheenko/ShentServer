#ifndef REQUEST_PROP_H
#define REQUEST_PROP_H

#include <string>

struct RequestProp {
	unsigned version = 11;
	std::string user_agent;
};



#endif //REQUEST_PROP_H
