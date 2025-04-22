#include "AuthTokens.hpp"

#include "../../Crypto/CryptoManager.h"


AuthTokens AuthTokens::create(int user_id, int ttl_sec) {
	CryptoManager manager;
	AuthTokens tokens;

	tokens.access_token = manager.jwt_generate("access", "auth", user_id, ttl_sec);
	tokens.refresh_token = manager.uuid_generate();

	return tokens;
}

void to_json(nlohmann::json &j, const AuthTokens &p) {
	j = nlohmann::json{{"access_token", p.access_token}, {"refresh_token", p.refresh_token}};
}

void from_json(const nlohmann::json& j, AuthTokens& p) {
	j.at("access_token").get_to(p.access_token);
	j.at("refresh_token").get_to(p.refresh_token);
}

