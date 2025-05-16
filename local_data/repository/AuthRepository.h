#ifndef AUTHREPOSITORY_H
#define AUTHREPOSITORY_H

#include <memory>
#include "../RedisClient.h"
#include "../model/RefreshTokenStorage.h"

// TODO: Перенести методы авторизации сюда, а то че за херня
class AuthRepository {
	std::shared_ptr<RedisClient> redis_client_;

public:
	explicit AuthRepository(const std::shared_ptr<RedisClient> &redis_client);

	std::optional<RefreshTokenStorage> get_refresh_token_storage(int user_id);
	void set_refresh_token_storage(int user_id, RefreshTokenStorage &token_storage, int ttl);

	void add_refresh_token(int user_id, std::string tokent_sheet, int ttl_sec);
	void remove_refresh_token(int user_id);
	void ban_refresh_token(int user_id, int ttl_sec);
	bool validate_refresh_token(int user_id, std::string hash_token);

private:
	std::optional<std::string> get_token_sheet(int user_id);
	std::optional<std::string> get_ban_refresh_token(int user_id);
};



#endif //AUTHREPOSITORY_H
