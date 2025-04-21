#ifndef USER_H
#define USER_H

#include <libpq-fe.h>
#include <optional>
#include <string>

struct User {
	int id;
	std::string login;
	std::string name;

	static std::optional<User> fromPGResult(PGresult* result, int row);
};



#endif //USER_H
