#include "JsonConfigParser.h"
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;
using json = nlohmann::json;


std::optional<Config> JsonConfigParser::LoadFromFile(const std::string &path) {
	try {
		std::ifstream f(path);
		if (!f.is_open())
			return std::nullopt;

		json j = json::parse(f);

		Config config;
		config.pqSettings = ParsePqSettings(j.at("postgres"));
		config.redisSettings = ParseRedisSettings(j.at("redis"));

		return config;
	} catch (...) {
		return std::nullopt;
	}
}

PqSettings JsonConfigParser::ParsePqSettings(const nlohmann::json &j) {
	PqSettings settings;
	settings.dbname = j.at("dbname").get<std::string>();
	settings.hostaddr = j.at("hostaddr").get<std::string>();
	settings.password = j.at("password").get<std::string>();
	settings.user = j.at("user").get<std::string>();
	settings.port = j.at("port").get<int>();

	return settings;
}

RedisSettings JsonConfigParser::ParseRedisSettings(const nlohmann::json &j) {
	RedisSettings settings;

	settings.host = j.at("host").get<std::string>();
	settings.password = j.at("password").get<std::string>();
	settings.port = j.at("port").get<int>();

	return settings;
}
