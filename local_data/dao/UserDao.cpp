#include "UserDao.h"

UserDao::UserDao(PostgresClient &db) : db_(db) {}

void UserDao::get_auth_data(int id, Callback<UserAuth> callback) {
	const std::string query = std::format("SELECT users.id, user_auth.password_hash, user_auth.salt\n"
	"FROM users\n"
	"LEFT JOIN user_auth ON users.id = user_auth.user_id\n"
	"WHERE users.id = {}", id);

	db_.asyncQuery(query, [callback, query](PGresult* result) {
		if (!result || PQntuples(result) == 0) {
			callback(std::nullopt);
			return;
		}

		callback(UserAuth::fromPGResult(result));
	});
}

void UserDao::get_user(int id, Callback<User> callback) {
	const std::string query = std::format("SELECT id, login, name FROM users WHERE id = {}", id);

	db_.asyncQuery(query, [callback, query](PGresult* result) {
		if (!result || PQntuples(result) == 0) {
			callback(std::nullopt);
			return;
		}

		callback(User::fromPGResult(result, 0));
	});
}

void UserDao::get_user(std::string login, Callback<User> callback) {
	const std::string query = std::format("SELECT id, login, name FROM users WHERE login = '{}'", login);

	db_.asyncQuery(query, [callback, query](PGresult* result) {
		if (!result || PQntuples(result) == 0) {
			callback(std::nullopt);
			return;
		}

		callback(User::fromPGResult(result, 0));
	});
}
