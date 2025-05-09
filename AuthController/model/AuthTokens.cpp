#include "AuthTokens.hpp"

#include "../../Crypto/CryptoManager.h"


AuthTokens AuthTokens::create(int user_id, int ttl_sec) {
	CryptoManager manager;
	AuthTokens tokens;

	tokens.access_token = manager.jwt_generate("access", "auth", user_id, ttl_sec);
	tokens.refresh_token = manager.uuid_generate();

	return tokens;
}

std::string AuthTokens::create_refresh_sheet(const std::string &device_id, const std::string &user_agent) {
	std::string salt =  CryptoManager::salt64_generate();
	nlohmann::json j = {
		{"refresh_token_hash", CryptoManager::hash(this->refresh_token, salt)},
		{"device_id", CryptoManager::hash(device_id, salt)},
		{"user_agent", user_agent},
		{"salt", salt},
	};

	return j.dump();
}

void to_json(nlohmann::json &j, const AuthTokens &p) {
	j = nlohmann::json{{"access_token", p.access_token}, {"refresh_token", p.refresh_token}};
}

void from_json(const nlohmann::json& j, AuthTokens& p) {
	j.at("access_token").get_to(p.access_token);
	j.at("refresh_token").get_to(p.refresh_token);
}

