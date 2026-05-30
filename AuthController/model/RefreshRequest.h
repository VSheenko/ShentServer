#ifndef REFRESHREQUEST_H
#define REFRESHREQUEST_H

#include <optional>
#include <string>

struct RefreshRequest {
	int user_id;
	std::string device_id;
	std::string refresh_token;

	static std::optional<RefreshRequest> from_json(std::string s_obj);
};



#endif //REFRESHREQUEST_H
