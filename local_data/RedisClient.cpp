#include "RedisClient.h"

namespace redis = sw::redis;

RedisClient::RedisClient(const RedisSettings &settings, int pool_size) {
	redis::ConnectionOptions redis_options;

	redis_options.host = settings.host;
	redis_options.port = settings.port;
	redis_options.password = settings.password;
	redis_options.socket_timeout = std::chrono::milliseconds(200);

	redis::ConnectionPoolOptions pool_options;
	pool_options.size = pool_size;

	redis_ = std::make_unique<redis::Redis>(redis_options, pool_options);

	if (redis_->ping() != "PONG")
		throw std::runtime_error("");

}

void RedisClient::set(const std::string &key, const std::string &value) {
	redis_->set(key, value);
}

void RedisClient::set(const std::string &key, const std::string &value, int ttl_sec) {
	redis_->set(key, value, std::chrono::seconds(ttl_sec));
}

void RedisClient::del(const std::string &key) {
	redis_->del(key);
}

std::optional<std::string> RedisClient::get(const std::string &key) {
	if (auto res = redis_->get(key))
		return *res;

	return std::nullopt;
}
