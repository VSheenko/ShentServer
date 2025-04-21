#ifndef LOGIN_DATA_H
#define LOGIN_DATA_H

#include <optional>
#include <string>

struct crypt_data {
	std::string data;

	static std::optional<crypt_data> from_json(std::string s_json);
};



#endif //LOGIN_DATA_H
