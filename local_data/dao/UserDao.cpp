#include "UserDao.h"

UserDao::UserDao(PostgresClient &db) : db_(db) {}

void UserDao::get_auth_data(int id, Callback<UserAuth> callback) {
	const std::string query = std::format("SELECT users.id, user_auth.password_hash, user_auth.salt, user_auth.private_key\n"
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

void UserDao::insert_user(const User &user, Callback<int>callback) {
	const std::string query = std::format("INSERT INTO users (login, name)\n"
	"VALUES ('{}', '{}')\n"
	"RETURNING id;", user.login, user.name);

	db_.asyncQuery(query, [callback](PGresult* result) {
		if (!result || PQntuples(result) != 1) {
			callback(std::nullopt);
			return;
		}

		callback(std::stoi(PQgetvalue(result, 0, 0)));
	});
}


void UserDao::insert_user_auth(const UserAuth &user_auth, std::function<void(bool)> callback) {
	const std::string query = std::format("INSERT INTO user_auth (user_id, password_hash, salt, private_key)\n"
									   "VALUES ({}, '{}', '{}', '{}');", user_auth.id, user_auth.password_hash, user_auth.salt,
									   user_auth.private_key);

	db_.asyncQuery(query, [callback](PGresult* result) {
		if (!result) {
			callback(false);
			return;
		}

		callback(true);
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
