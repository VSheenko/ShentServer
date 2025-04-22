#ifndef CHATDAO_H
#define CHATDAO_H

#include <functional>
#include <optional>

#include "../PostgresClient.h"
#include "../model/User.h"
#include "../model/UserAuth.hpp"

class UserDao {
	template<typename T>
	using Callback = std::function<void(std::optional<T>)>;

	PostgresClient& db_;

public:
	explicit UserDao(PostgresClient& db);

	void get_user(int id, Callback<User> callback);
	void get_user(std::string login, Callback<User> callback);

	void get_auth_data(int id, Callback<UserAuth> callback);
};



#endif //CHATDAO_H
