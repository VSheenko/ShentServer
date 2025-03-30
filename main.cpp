#include <iostream>

#include "FrontController/server.h"
#include "UserController/user_handler.h"

#define BOOST_ASIO_ENABLE_HANDLER_TRACKING

int main(int argc, char* argv[]) {
	router rt;
	rt.add_route("/user", std::make_shared<user_handler>());


	server shentserver(std::stoi(argv[1]), std::thread::hardware_concurrency(), rt);

	shentserver.run();
}
