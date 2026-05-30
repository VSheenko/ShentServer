#include "UserRepository.h"

UserRepository::UserRepository(std::shared_ptr<PostgresClient> db) {
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

void UserRepository::async_create_user(const User &user, UserAuth &user_auth, std::function<void(int)> callback) {
	if (!callback) {
		std::cout << "Empty callback" << std::endl;
		return;
	}

	dao_->insert_user(user, [this, callback, user_auth](std::optional<int> id_opt) mutable {
		if (!id_opt.has_value()) {
			callback(-1);
			return;
		}

		user_auth.id = *id_opt;
		dao_->insert_user_auth(user_auth, [callback, user_auth](bool res) {
			if (!res) {
				callback(-1);
				return;
			}

			callback(user_auth.id);
			return;
		});
	});
}

void UserRepository::async_get_salt(const std::string &login, std::function<void(std::optional<std::string>)> callback) {
	dao_->get_salt(login, [callback](std::optional<std::string> data) {
		callback(data);
	});
}

void UserRepository::async_get_auth_data(int id, std::function<void(std::optional<UserAuth>)> callback) {
	dao_->get_auth_data(id, [callback](std::optional<UserAuth> data) {
		callback(data);
	});
}
