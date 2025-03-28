#include "FrontController.h"

namespace asio = boost::asio;

FrontController::FrontController(const short port): server_(port, std::thread::hardware_concurrency()) {

}

void FrontController::run() { server_.run(); }
