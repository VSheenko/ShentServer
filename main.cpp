#include <iostream>

#include "FrontController/server.h"
#include "UserController/user_handler.h"

#include "local_data/JsonConfigParser.h"
#include "local_data/ShentDB.h"
#include "local_data/repository/MessageRepository.h"
#include "local_data/repository/UserRepository.h"
#include "WebSocketServer/websocket_handler.h"


int main(int argc, char* argv[]) {
	router rt;
	rt.add_route("/user", std::make_shared<user_handler>());
	rt.add_route("/ws", std::make_shared<websocket_handler>());

	server shentserver(std::stoi(argv[1]), std::thread::hardware_concurrency(), rt);

	auto shent_db = std::make_shared<ShentDB>(shentserver.getContext(), JsonConfigParser::LoadFromFile("./config.json")->pqSettings.getConnString(), 10);
	UserRepository user_repository(shent_db);
	MessageRepository message_repository(shent_db);

	user_repository.async_get(1, [](std::optional<User> user) {
		if (user) {
			std::cout << "User: " << user->name << " (" << user->tag << ")" << std::endl;
		} else {
			std::cout << "User NOT FOUND" << '\n';
		}
	});

	shentserver.run();
}
