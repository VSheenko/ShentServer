#ifndef ROUTER_H
#define ROUTER_H

#include <string>
#include <unordered_map>

#include "base_handler.h"


class router {
public:
	using handler_ptr = std::shared_ptr<base_handler>;

	using response_t = boost::beast::http::response<boost::beast::http::string_body>;
	using request_t = boost::beast::http::request<boost::beast::http::string_body>;
	using socket_t = boost::asio::ip::tcp::socket;
	using response_handler = std::function<void(response_t)>;

	router() = default;
	explicit router(std::vector<std::pair<std::string, handler_ptr>>& handlers);
	void add_route(std::string path, handler_ptr handler);

	std::optional<response_t> handle_request(const request_t& req, socket_t& socket);
	void async_handle_request(const request_t& req, socket_t& socket, response_handler   on_response);
private:
	std::pmr::unordered_map<std::string, handler_ptr> routes_;
};



#endif //ROUTER_H
