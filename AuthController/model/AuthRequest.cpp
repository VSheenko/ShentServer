#include "AuthRequest.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

std::optional<AuthRequest> AuthRequest::from_json(const std::string &s_json) {
	try {
		json j = json::parse(s_json);

		AuthRequest data;

		j["pub_key"].get_to(data.pub_key);
		j["login"].get_to(data.login);
		j["password"].get_to(data.password);
		j["timestamp"].get_to(data.timestamp);
		j["nonce"].get_to(data.nonce);

		return data;
	} catch (...) {
		return std::nullopt;
	}
}
