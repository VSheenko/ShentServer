#include "router.h"

namespace http = boost::beast::http;

router::router(std::vector<std::pair<std::string, handler_ptr>> &handlers) {
	for (auto& [path, handler] : handlers) {
		routes_[path] = std::move(handler);
	}
}

void router::add_route(std::string path, handler_ptr handler) {
	routes_[path] = std::move(handler);
}

std::optional<http::response<http::string_body>> router::handle_request( const http::request<http::string_body> &req,
														  boost::asio::ip::tcp::socket& socket) {

	std::string s_target_handler = std::string(req.target());
	size_t pos = s_target_handler.find('/', 1);

	if (pos != std::string::npos)
		s_target_handler = s_target_handler.substr(0, s_target_handler.find('/', 1));

	auto it = routes_.find(s_target_handler);
	if (it != routes_.end())
		return it->second->handle_request(req, socket);

	http::response<http::string_body> response;
	response.result(http::status::not_found);
	response.body() = R"({"error": "Not found"})";
	response.prepare_payload();
	return response;
}
