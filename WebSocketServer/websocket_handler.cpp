#include "websocket_handler.h"
#include "WebSocketSession.h"
#include <nlohmann/json.hpp>
#include "../local_data/model/request_target.h"


namespace http = boost::beast::http;
using json = nlohmann::json;

websocket_handler::websocket_handler() {
	manager_ = std::make_shared<SessionManager>();
}

std::optional<http::response<http::string_body>> websocket_handler::handle_request(
	const http::request<http::string_body> &req, boost::asio::ip::tcp::socket &socket) {

	http::response<http::string_body> res;
	res.version(req.version());
	res.set(http::field::server, "Shent.User");
	res.set(http::field::content_type, "application/json");

	request_target target(req.target());

	if (target.path == "/ws/connect" &&
		req.method() == http::verb::get &&
		req.find(http::field::upgrade) != req.end() &&
		req.at(http::field::upgrade) == "websocket") {
		auto session = std::make_shared<WebSocketSession>(std::move(socket), manager_);

		try {
			session->run(req);
			return std::nullopt;
		} catch (std::exception& e) {
			res.result(http::status::bad_request);
			res.body() = "{\"error\": \"" + std::string(e.what()) +  "\"}";
		}
	} else if (target.path == "/ws/get_connection") {
		res.result(http::status::ok);
		std::string s = "{\n";
		for (auto& i : manager_->get_users()) {
			s += (R"({"user": )" + std::to_string(i) + "},\n");
		}

		s += "}";
		res.body() = s;
	} else {
		res.result(http::status::bad_request);
		res.body() = R"({"error": ""})";
	}


	res.prepare_payload();
	return res;

}
