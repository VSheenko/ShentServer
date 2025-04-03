#ifndef USERREPOSITORY_H
#define USERREPOSITORY_H

#include "../dao/UserDao.h"

class UserRepository {
	std::shared_ptr<UserDao> dao_;

public:
	explicit UserRepository(ShentDB& db);

	void async_get(int id, std::function<void(std::optional<User>)> callback);
	void async_get(std::string tag, std::function<void(std::optional<User>)> callback);
};



#endif //USERREPOSITORY_H
