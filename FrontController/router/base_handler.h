#ifndef BASEHANDLER_H
#define BASEHANDLER_H

#include <boost/beast/http.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <optional>

class base_handler {
public:
	virtual ~base_handler() = default;
	virtual std::optional<boost::beast::http::response<boost::beast::http::string_body>> handle_request(
		const boost::beast::http::request<boost::beast::http::string_body> &req, boost::asio::ip::tcp::socket &socket) = 0;
};

#endif //BASEHANDLER_H
