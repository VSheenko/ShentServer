#include "User.h"

std::optional<User> User::fromPGResult(PGresult *result, int row) {
	if (!result || PQntuples(result) <= row)
		return std::nullopt;

	return User {
		std::stoi(PQgetvalue(result, row, 0)),
		PQgetvalue(result, row, 1),
		PQgetvalue(result, row, 2)
	};
}
