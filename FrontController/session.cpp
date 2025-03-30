#include "session.h"

#include <iostream>

namespace beast = boost::beast;
namespace http = beast::http;
namespace asio = boost::asio;
using tcp = asio::ip::tcp;

session::session(boost::asio::ip::tcp::socket socket, const router& rt)
    : socket_(std::move(socket)), router_(std::move(rt)) {}

void session::start() { read_request(); }

void session::read_request() {
      http::async_read(socket_, buffer_, request_,
                       [self = shared_from_this()](beast::error_code ec, std::size_t) {
                             if (!ec) {
                               self->processes_request();
                             } else {
                                 std::cerr << "Error [server::read_request]: " << ec << std::endl;
                             }
                       });
}


void session::processes_request() {
    http::response<http::string_body> response = router_.handle_request(request_);

    send_response(std::move(response));
}

void session::send_response(http::response<http::string_body> response) {
    auto shared_response = std::make_shared<decltype(response)>(std::move(response));

    http::async_write(socket_, *shared_response,
        [self = shared_from_this(), shared_response](beast::error_code ec, std::size_t) {
            if (ec) {
                std::cerr << "Error [session::send_response]: " << ec << std::endl;
            }

            self->close_connection();
        });
}

void session::close_connection() {
    beast::error_code ec;
    socket_.shutdown(tcp::socket::shutdown_both, ec);

    socket_.close(ec);
}

