#ifndef REDISDB_H
#define REDISDB_H

#include "model/RedisSettings.h"
#include <sw/redis++/redis++.h>


class RedisClient {
	std::unique_ptr<sw::redis::Redis> redis_;

public:
	explicit RedisClient(const RedisSettings &settings, int pool_size);

	void set(const std::string& key, const std::string& value);
	void set(const std::string& key, const std::string& value, int ttl_sec);
	void del(const std::string& key);

	std::optional<std::string> get(const std::string& key);
};



#endif //REDISDB_H
