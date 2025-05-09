#include "User.h"

User::User(const std::string &login, const std::string &name)
	: id(0), login(login), name(name) {}

User::User(int id, const std::string &login, const std::string &name)
		: id(id), login(login), name(name) {}

std::optional<User> User::fromPGResult(PGresult *result, int row) {
	if (!result || PQntuples(result) <= row)
		return std::nullopt;

	return User {
		std::stoi(PQgetvalue(result, row, 0)),
		PQgetvalue(result, row, 1),
		PQgetvalue(result, row, 2)
	};
}
