#include "base_handler.h"
#include <boost/url.hpp>

void base_handler::async_handle_request(const request_t &req, socket_t &socket, response_handler on_response) {
	auto response = handle_request(req, socket);
	if (response.has_value())
		on_response(std::move(response.value()));
}


void base_handler::set_bad_response(response_t &response,
									const boost::beast::http::status status, std::string message, const response_handler &on_response) {
	response.result(status);
	response.body() = ("{\"error\": \"" + message + "\"}");
	response.prepare_payload();
	on_response(std::move(response));
}

std::unordered_map<std::string, std::string> base_handler::get_params(const request_t &request) {
	boost::urls::url_view url_view(request.target());
	std::unordered_map<std::string, std::string> map;

	for (auto const& param: url_view.params()) {
		map[param.key] = param.value;
	}

	return map;
}
