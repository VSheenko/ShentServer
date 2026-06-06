#include <iostream>

#include "AccountController/account_handler.h"
#include "AuthController/auth_handler.h"
#include "FrontController/server.h"
#include "UserController/user_handler.h"

#include "local_data/JsonConfigParser.h"
#include "local_data/PostgresClient.h"
#include "local_data/ShentDB.h"
#include "local_data/repository/MessageRepository.h"
#include "local_data/repository/UserRepository.h"
#include "WebSocketServer/websocket_handler.h"

static const std::string CONFIG_PATH = "./config.json"

int main(int argc, char* argv[]) {
	auto rt = std::make_shared<router>();
	server shentserver(std::stoi(argv[1]), std::thread::hardware_concurrency(), rt);
	auto pq = std::make_shared<PostgresClient>(shentserver.getContext(), JsonConfigParser::LoadFromFile(CONFIG_PATH)->pqSettings.getConnString(), 10);
	auto redis = std::make_shared<RedisClient>(JsonConfigParser::LoadFromFile(CONFIG_PATH)->redisSettings, 5);
	auto shent_db = std::make_shared<ShentDB>(pq, redis);
	auto crypto = std::make_shared<CryptoManager>();

	auto user_repository = std::make_shared<UserRepository>(shent_db->pq());
	auto auth_repository = std::make_shared<AuthRepository>(shent_db->redis());

	rt->add_route("/user", std::make_shared<user_handler>());
	rt->add_route("/ws", std::make_shared<websocket_handler>(crypto));
	rt->add_route("/api", std::make_shared<auth_handler>(user_repository, auth_repository));
	rt->add_route("/account", std::make_shared<account_handler>(crypto, user_repository));


	MessageRepository message_repository(shent_db->pq());

	user_repository->async_get("VSheenko", [](std::optional<User> user) {
		if (user) {
			std::cout << "User: " << user->name << " (" << user->login << ")" << std::endl;
		} else {
			std::cout << "User NOT FOUND" << '\n';
		}
	});

	shentserver.run();
}
