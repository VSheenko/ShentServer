#ifndef REFRESHTOKENSTORAGE_H
#define REFRESHTOKENSTORAGE_H

#include <string>
#include <nlohmann/json.hpp>

struct RefreshTokenStorage {
	std::string device_id;
	std::string refresh_token_hash;
	std::string salt;
	std::string user_agent;

	std::string get_sjson();
};

void from_json(const nlohmann::json& j, RefreshTokenStorage& p);
void to_json(nlohmann::json& j, const RefreshTokenStorage& p);



#endif //REFRESHTOKENSTORAGE_H
