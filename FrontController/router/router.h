#ifndef ROUTER_H
#define ROUTER_H

#include <string>
#include <unordered_map>

#include "base_handler.h"


class router {
public:
	using handler_ptr = std::shared_ptr<base_handler>;

	router() = default;
	explicit router(std::vector<std::pair<std::string, handler_ptr>>& handlers);
	void add_route(std::string path, handler_ptr handler);

	std::optional<boost::beast::http::response<boost::beast::http::string_body>> handle_request(
		const boost::beast::http::request<boost::beast::http::string_body>& req, boost::asio::ip::tcp::socket& socket);
private:
	std::pmr::unordered_map<std::string, handler_ptr> routes_;
};



#endif //ROUTER_H
