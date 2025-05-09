#ifndef REGISTERREQUEST_H
#define REGISTERREQUEST_H

#include <optional>
#include <string>

class RegisterRequest {
public:
	std::string login;
	std::string password;
	std::string salt;
	std::string private_key;
	long timestamp;

	static std::optional<RegisterRequest> from_json(const std::string &s);
};



#endif //REGISTERREQUEST_H
