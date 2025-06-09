#ifndef BASEHANDLER_H
#define BASEHANDLER_H

#include <boost/beast/http.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <optional>

class base_handler {
public:
	using response_t = boost::beast::http::response<boost::beast::http::string_body>;
	using request_t = boost::beast::http::request<boost::beast::http::string_body>;
	using socket_t = boost::asio::ip::tcp::socket;
	using response_handler = std::function<void(response_t)>;

	virtual ~base_handler() = default;
	virtual std::optional<response_t> handle_request(const request_t &req, socket_t &socket) = 0;
	virtual void async_handle_request(const request_t& req, socket_t& socket, response_handler on_response);

protected:
	void set_bad_response(response_t& response,
	                      boost::beast::http::status status, std::string message, const response_handler &on_response);
	std::unordered_map<std::string, std::string> get_params(const request_t &request);
};





#endif //BASEHANDLER_H
