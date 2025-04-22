#ifndef USERREPOSITORY_H
#define USERREPOSITORY_H

#include "../dao/UserDao.h"

class UserRepository {
	std::shared_ptr<UserDao> dao_;

public:
	explicit UserRepository(std::shared_ptr<PostgresClient> db);

	void async_get(int id, std::function<void(std::optional<User>)> callback);
	void async_get(std::string login, std::function<void(std::optional<User>)> callback);

	void async_get_auth_data(int id, std::function<void(std::optional<UserAuth>)> callback);
};



#endif //USERREPOSITORY_H
