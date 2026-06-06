#include "account_handler.h"
#include <iostream>
#include <nlohmann/json.hpp>

#include "../local_data/model/request_target.h"

namespace http = boost::beast::http;

account_handler::account_handler(const std::shared_ptr<CryptoManager> &crypto_manager,
	const std::shared_ptr<UserRepository>& user_repository)
	: protect_handler(crypto_manager), user_repository_(user_repository) {}

std::optional<base_handler::response_t> account_handler::handle_request(const request_t &req, socket_t &socket) {
	response_t response;
	return response;
}

void account_handler::async_handle_request(const request_t &req, socket_t &socket, response_handler on_response)
{
	response_t response;
	response.version(req.version());
	response.set(http::field::server, "Shent.User");
	response.set(http::field::content_type, "application/json");

	int user_id = authorize(req);
	if (user_id == -1) {
		set_bad_response(response, http::status::unauthorized, "unauthorized", on_response);
		return;
	}

	request_target target(req.target());
	if (target.path == "/account/user" && req.method() == http::verb::get) {
		std::string login = target.params["login"];

		user_repository_->async_get(login, [this, response, on_response](std::optional<User> user_opt) mutable {
			if (!user_opt.has_value()) {
				set_bad_response(response, http::status::not_found, "User not found", on_response);
				return;
			}

			nlohmann::json j;
			j["id"] = user_opt->id;
			j["login"] = user_opt->login;
			j["name"] = user_opt->name;

			response.body() = j.dump();

			on_response(std::move(response));
		});
	}
}
