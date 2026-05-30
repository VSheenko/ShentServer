#ifndef REDIS_SETTINGS_H
#define REDIS_SETTINGS_H
#include <string>

struct RedisSettings {
	std::string host;
	std::string password;
	int port;
};

#endif //REDIS_SETTINGS_H
