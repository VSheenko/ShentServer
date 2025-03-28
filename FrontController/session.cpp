#include "session.h"

#include <iostream>

namespace beast = boost::beast;
namespace http = beast::http;
namespace asio = boost::asio;
using tcp = asio::ip::tcp;

session::session(boost::asio::ip::tcp::socket socket)
    : socket_(std::move(socket)) {}

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
    http::response<http::string_body> response;
    response.version(request_.version());
    response.set(http::field::server, "Boost.Beast");
    response.set(http::field::content_type, "text/plain");

    if (request_.method() == http::verb::get) {
        response.result(http::status::ok);
        response.body() = "GET request received!";
    }
    else if (request_.method() == http::verb::post) {
        response.result(http::status::ok);
        response.body() = "POST data: " + (request_.body().empty() ? "No body" : request_.body());
    }
    else {
        response.result(http::status::bad_request);
        response.body() = "Unsupported HTTP method";
    }

    response.prepare_payload();
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

