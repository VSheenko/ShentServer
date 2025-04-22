#ifndef AUTHREPOSITORY_H
#define AUTHREPOSITORY_H

#include <memory>
#include "../RedisClient.h"


class AuthRepository {
	std::shared_ptr<RedisClient> redis_client_;

public:
	explicit AuthRepository(const std::shared_ptr<RedisClient> &redis_client);

	void add_refresh_token(int user_id, std::string token, int ttl_sec);
	void remove_refresh_token(int user_id);
	void ban_refresh_token(int user_id, int ttl_sec);
	bool validate_refresh_token(int user_id, std::string token);

private:
	std::optional<std::string> get_refresh_token(int user_id);
	std::optional<std::string> get_ban_refresh_token(int user_id);
};



#endif //AUTHREPOSITORY_H
