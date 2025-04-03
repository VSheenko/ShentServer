#ifndef CHATDAO_H
#define CHATDAO_H

#include <functional>
#include <optional>

#include "../ShentDB.h"
#include "../model/User.h"

class UserDao {
	using Callback = std::function<void(std::optional<User>)>;

	ShentDB& db_;

public:
	explicit UserDao(ShentDB& db);

	void getById(int id, Callback callback);
	void getByTag(std::string tag, Callback callback);
};



#endif //CHATDAO_H
