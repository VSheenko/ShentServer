#include "AuthRepository.h"

AuthRepository::AuthRepository(const std::shared_ptr<RedisClient> &redis_client) : redis_client_(redis_client) {}

void AuthRepository::add_refresh_token(int user_id, std::string tokent_sheet, int ttl_sec) {
	redis_client_->set(std::format("refresh:{}", user_id), tokent_sheet, ttl_sec);
}

void AuthRepository::remove_refresh_token(int user_id) {
	redis_client_->del(std::format("refresh:{}", user_id));
}

void AuthRepository::ban_refresh_token(int user_id, int ttl_sec) {
	remove_refresh_token(user_id);

	std::optional<std::string> token_opt = get_token_sheet(user_id);
	if (!token_opt.has_value())
		return;

	redis_client_->set(std::format("ban:refresh:{}", user_id), token_opt.value(), ttl_sec);
}

bool AuthRepository::validate_refresh_token(int user_id, std::string token) {
	std::optional<std::string> ban_opt = get_ban_refresh_token(user_id);
	if (ban_opt.has_value() && ban_opt.value() == token)
		return false;

	std::optional<std::string> unban_opt = get_token_sheet(user_id);
	if (unban_opt.has_value() && unban_opt.value() == token)
		return true;

	return false;
}

std::optional<std::string> AuthRepository::get_token_sheet(int user_id) {
	return redis_client_->get(std::format("refresh:{}", user_id));
}

std::optional<std::string> AuthRepository::get_ban_refresh_token(int user_id) {
	return redis_client_->get(std::format("ban:refresh:{}", user_id));
}



