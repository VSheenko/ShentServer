//
// Created by vovan on 03.04.2025.
//

#include "UserDao.h"

UserDao::UserDao(ShentDB &db) : db_(db) {}

void UserDao::getById(int id, Callback callback) {
	const std::string query = std::format("SELECT id, tag, name FROM users WHERE id = {}", id);

	db_.asyncQuery(query, [callback](PGresult* result) {
		if (!result || PQntuples(result) == 0) {
			callback(std::nullopt);
			return;
		}

		callback(User::fromPGResult(result, 0));
	});
}

void UserDao::getByTag(std::string tag, Callback callback) {
	const std::string query = std::format("SELECT id, tag, name FROM users WHERE tag = {}", tag);

	db_.asyncQuery(query, [callback](PGresult* result) {
		if (!result || PQntuples(result) == 0) {
			callback(std::nullopt);
			return;
		}

		callback(User::fromPGResult(result, 0));
	});
}
