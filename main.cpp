#include <iostream>

#include "AuthController/auth_handler.h"
#include "FrontController/server.h"
#include "UserController/user_handler.h"

#include "local_data/JsonConfigParser.h"
#include "local_data/ShentDB.h"
#include "local_data/repository/MessageRepository.h"
#include "local_data/repository/UserRepository.h"
#include "WebSocketServer/websocket_handler.h"


int main(int argc, char* argv[]) {
	auto rt = std::make_shared<router>();
	server shentserver(std::stoi(argv[1]), std::thread::hardware_concurrency(), rt);
	auto shent_db = std::make_shared<ShentDB>(shentserver.getContext(), JsonConfigParser::LoadFromFile("./config.json")->pqSettings.getConnString(), 10);
	auto user_repository = std::make_shared<UserRepository>(shent_db);

	rt->add_route("/user", std::make_shared<user_handler>());
	rt->add_route("/ws", std::make_shared<websocket_handler>());
	rt->add_route("/api", std::make_shared<auth_handler>(user_repository));



	MessageRepository message_repository(shent_db);

	user_repository->async_get("VSheenko", [](std::optional<User> user) {
		if (user) {
			std::cout << "User: " << user->name << " (" << user->login << ")" << std::endl;
		} else {
			std::cout << "User NOT FOUND" << '\n';
		}
	});

	shentserver.run();
}
