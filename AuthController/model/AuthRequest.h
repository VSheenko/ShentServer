#ifndef AUTH_DATA_H
#define AUTH_DATA_H

#include <optional>
#include <string>

struct AuthRequest {
	std::string pub_key;
	std::string login;
	std::string password;
	std::string device_id;
	long timestamp;
	std::string nonce;

	static std::optional<AuthRequest> from_json(const std::string& s_json);
};



#endif //AUTH_DATA_H
