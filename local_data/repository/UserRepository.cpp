#include "UserRepository.h"

UserRepository::UserRepository(std::shared_ptr<ShentDB> db) {
	dao_ = std::make_shared<UserDao>(*db);
}

void UserRepository::async_get(int id, std::function<void(std::optional<User>)> callback) {
	dao_->getById(id, [callback](std::optional<User> user) {
		callback(user);
	});
}

void UserRepository::async_get(std::string tag, std::function<void(std::optional<User>)> callback) {
	dao_->getByTag(tag, [callback](std::optional<User> user) {
		callback(user);
	});
}
