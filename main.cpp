#include <iostream>

#include "FrontController/server.h"

#define BOOST_ASIO_ENABLE_HANDLER_TRACKING

int main(int argc, char* argv[]) {

	try {
		server server(std::stoi(argv[1]), std::thread::hardware_concurrency());
		std::cout << "Server running on port " << std::stoi(argv[1]) << "...\n";
		std::this_thread::sleep_for(std::chrono::hours(24));
	} catch (const std::exception& e) {
		std::cerr << "Server error: " << e.what() << std::endl;
	}
}
