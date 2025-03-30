#include "server.h"
#include <iostream>

namespace asio = boost::asio;
namespace beast = boost::beast;
using tcp = asio::ip::tcp;

server::server(short port, int thread_count, const router& rt)
    : acceptor_(io_context_, tcp::endpoint(tcp::v4(), port)), router_(std::move(rt)) {
    accept_connections();

    for (int i = 0; i < thread_count; i++) {
        workers_.emplace_back([this] { io_context_.run(); });
    }
}
server::~server() {
    io_context_.stop();
    for (auto& worker : workers_) {
        if (worker.joinable()) worker.join();
    }
}

void server::accept_connections() {
    acceptor_.async_accept(
        [this](beast::error_code ec, tcp::socket socket) {
            if (!ec) {
                std::make_shared<session>(std::move(socket), router_)->start();
                accept_connections();
            }
        }
    );
}
