#include "RefreshTokenStorage.h"
#include <nlohmann/json.hpp>


std::string RefreshTokenStorage::get_sjson() {
	nlohmann::json j = *this;
	return j.dump();

}

void from_json(const nlohmann::json &j, RefreshTokenStorage &p) {
	j.at("device_id").get_to(p.device_id);
	j.at("refresh_token_hash").get_to(p.refresh_token_hash);
	j.at("salt").get_to(p.salt);
	j.at("user_agent").get_to(p.user_agent);
}

void to_json(nlohmann::json &j, const RefreshTokenStorage &p) {
	j["device_id"] = p.device_id;
	j["refresh_token_hash"] = p.refresh_token_hash;
	j["salt"] = p.salt;
	j["user_agent"] = p.user_agent;
}
