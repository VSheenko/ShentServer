#ifndef USER_AUTH_H
#define USER_AUTH_H

#include <iostream>
#include <string>
#include <optional>
#include <libpq-fe.h>

struct UserAuth {
	int id;
	std::string password_hash;
	std::string salt;
	std::string private_key;

	static std::optional<UserAuth> fromPGResult(PGresult* result);
};

inline std::optional<UserAuth> UserAuth::fromPGResult(PGresult *result) {
	if (!result || PQntuples(result) == 0)
		return std::nullopt;

	if (PQntuples(result) > 1)
		std::cerr << "[user_auth::fromPGResult]: Lots of records in the db" << std::endl;

	return UserAuth {
		std::stoi(PQgetvalue(result, 0, 0)),
		PQgetvalue(result, 0, 1),
		PQgetvalue(result, 0, 2),
		PQgetvalue(result, 0, 3),
	};
}

#endif //USER_AUTH_H
