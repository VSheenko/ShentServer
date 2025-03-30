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

http::response<http::string_body> router::handle_request( const http::request<http::string_body> &req) {
	auto it = routes_.find(std::string(req.target()));
	if (it != routes_.end())
		return it->second->handle_request(req);

	http::response<http::string_body> response;
	response.result(http::status::not_found);
	response.body() = R"({"error": "Not found"})";
	response.prepare_payload();
	return response;
}
