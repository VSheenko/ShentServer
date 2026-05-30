#include "websocket_handler.h"
#include "WebSocketSession.h"
#include <nlohmann/json.hpp>
#include <iostream>
#include "../local_data/model/request_target.h"


namespace http = boost::beast::http;
using json = nlohmann::json;

websocket_handler::websocket_handler(std::shared_ptr<CryptoManager> crypto_manager)
 : protect_handler(crypto_manager) {
	manager_ = std::make_shared<SessionManager>();
}

std::optional<base_handler::response_t> websocket_handler::handle_request(const request_t &req, socket_t &socket) {}

void websocket_handler::async_handle_request(const request_t &req, socket_t &socket, response_handler on_response) {
	http::response<http::string_body> response;
	response.version(req.version());
	response.set(http::field::server, "Shent.User");
	response.set(http::field::content_type, "application/json");

	const int user_id = authorize(req);
	if (user_id == -1) {
		set_bad_response(response, http::status::unauthorized, "unauthorized", on_response);
		return;
	}

	request_target target(req.target());

	if (target.path == "/ws/connect" &&
	 req.method() == http::verb::get &&
	 req.find(http::field::upgrade) != req.end() &&
	 req.at(http::field::upgrade) == "websocket") {
		auto session = std::make_shared<WebSocketSession>(std::move(socket), manager_);

		try {
			session->run(req, user_id);
			return;
		} catch (std::exception& e) {
			std::cout << "[websocket_handler::async_handle_request]: ERROR - " << e.what() << std::endl;
			set_bad_response(response, http::status::internal_server_error, "Failed to start WebSocket session", on_response);
			return;
		}
	}

}
