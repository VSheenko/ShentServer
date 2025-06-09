#include "RegisterRequest.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

std::optional<RegisterRequest> RegisterRequest::from_json(const std::string &s) {
	try {
		json j = json::parse(s);

		RegisterRequest request;

		j["login"].get_to(request.login);
		j["password"].get_to(request.password);
		j["timestamp"].get_to(request.timestamp);
		j["private_key"].get_to(request.private_key);

		return request;
	} catch (...) {
		return std::nullopt;
	}
}
