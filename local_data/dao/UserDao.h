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

	void insert_user(const User& user, Callback<int>callback);
	void insert_user_auth(const UserAuth& user_auth, std::function<void(bool)> callback);
};



#endif //CHATDAO_H
