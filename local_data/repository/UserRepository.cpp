#include "UserRepository.h"

UserRepository::UserRepository(std::shared_ptr<ShentDB> db) {
	dao_ = std::make_shared<UserDao>(*db);
}

void UserRepository::async_get(int id, std::function<void(std::optional<User>)> callback) {
	dao_->get_user(id, [callback](std::optional<User> user) {
		callback(user);
	});
}

void UserRepository::async_get(std::string login, std::function<void(std::optional<User>)> callback) {
	dao_->get_user(login, [callback](std::optional<User> user) {
		callback(user);
	});
}

void UserRepository::async_get_auth_data(int id, std::function<void(std::optional<UserAuth>)> callback) {
	dao_->get_auth_data(id, [callback](std::optional<UserAuth> data) {
		callback(data);
	});
}
