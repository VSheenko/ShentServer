#include "account_handler.h"
#include <iostream>

namespace http = boost::beast::http;

account_handler::account_handler(const std::shared_ptr<CryptoManager> &crypto_manager)
	: protect_handler(crypto_manager, "auth") {

}

std::optional<base_handler::response_t> account_handler::handle_request(const request_t &req, socket_t &socket) {
	response_t response;
	return response;
}

void account_handler::async_handle_request(const request_t &req, socket_t &socket, response_handler on_response) {
	response_t response;
	response.version(req.version());
	response.set(http::field::server, "Shent.User");
	response.set(http::field::content_type, "application/json");

	int user_id = authorize(req);
	if (user_id == -1) {
		set_bad_response(response, http::status::unauthorized, "unauthorized", on_response);
		on_response(response);
		return;
	}
}
