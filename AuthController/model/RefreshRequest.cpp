#include "RefreshRequest.h"
#include <nlohmann/json.hpp>
#include <iostream>

std::optional<RefreshRequest> RefreshRequest::from_json(std::string s_obj) {
	try {
		RefreshRequest refresh_data;
		nlohmann::json j = nlohmann::json::parse(s_obj);

		j.at("user_id").get_to(refresh_data.user_id);
		j.at("device_id").get_to(refresh_data.device_id);
		j.at("refresh_token").get_to(refresh_data.refresh_token);

		return refresh_data;
	} catch (std::runtime_error& e) {
		std::cerr << "[RefreshRequest::from_json]: " << e.what() << std::endl;
		return std::nullopt;
	}
}
