#ifndef DB_SETTINGS_H
#define DB_SETTINGS_H
#include <string>


struct PqSettings {
	std::string hostaddr;
	int port;

	std::string dbname;
	std::string user;
	std::string password;

	std::string getConnString();
};



#endif //DB_SETTINGS_H
