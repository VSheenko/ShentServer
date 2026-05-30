#include "crypt_data.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

std::optional<crypt_data> crypt_data::from_json(std::string s_json) {
	try {
		json j = json::parse(s_json);

		crypt_data obj;

		j["data"].get_to(obj.data);

		return obj;
	} catch (...) {
		return std::nullopt;
	}
}
