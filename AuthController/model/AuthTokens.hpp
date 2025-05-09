#ifndef AUTHTOKEN_H
#define AUTHTOKEN_H

#include <string>
#include <nlohmann/json.hpp>

struct AuthTokens {
	std::string access_token;
	std::string refresh_token;

	static AuthTokens create(int user_id, int ttl_sec);
	std::string create_refresh_sheet(const std::string& device_id, const std::string& user_agent);
private:
	AuthTokens() = default;
};

void to_json(nlohmann::json& j, const AuthTokens& p);
void from_json(const nlohmann::json& j, AuthTokens& p);

#endif //AUTHTOKEN_H
