#include "user_handler.h"

namespace http = boost::beast::http;

http::response<http::string_body> user_handler::handle_request(const http::request<http::string_body> &req) {
	http::response<http::string_body> res;
	res.version(req.version());
	res.set(http::field::server, "Shent.User");
	res.set(http::field::content_type, "application/json");

	if (req.method() == http::verb::get) {
		res.result(http::status::ok);
		res.body() = R"({ "admin": "@VSheenko" })";
	} else {
		res.result(http::status::bad_request);
		res.body() = R"({"error": "Unsupported method"})";
	}

	res.prepare_payload();
	return res;
}
