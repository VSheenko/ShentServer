#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <nlohmann/json.hpp>
#include "model/PqSettings.h"
#include "model/RedisSettings.h"

struct Config {
	PqSettings pqSettings;
	RedisSettings redisSettings;
};

class JsonConfigParser {
public:
	static std::optional<Config> LoadFromFile(const std::string& path);

private:
	static PqSettings ParsePqSettings(const nlohmann::json& j);
	static RedisSettings ParseRedisSettings(const nlohmann::json& j);
};



#endif //CONFIG_H
