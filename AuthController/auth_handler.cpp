#include "auth_handler.h"
#include <cstdlib>
#include <iostream>
#include <nlohmann/json.hpp>

#include "../Crypto/CryptoManager.h"
#include "../local_data/PostgresClient.h"
#include "../local_data/model/request_target.h"
#include "../local_data/model/User.h"
#include "model/AuthRequest.h"
#include "model/crypt_data.h"



namespace http = boost::beast::http;
using json = nlohmann::json;

auth_handler::auth_handler(std::shared_ptr<UserRepository> user_repository, std::shared_ptr<AuthRepository> auth_repository): user_repository_(
	std::move(user_repository)), auth_repository_(auth_repository) {}

std::optional<http::response<http::string_body>> auth_handler::handle_request(
	const http::request<http::string_body> &req, boost::asio::ip::tcp::socket &socket) {

	CryptoManager crypto_manager;

	response_t response;
	response.version(req.version());
	response.set(http::field::server, "Shent.User");
	response.set(http::field::content_type, "application/json");

	request_target target(req.target());

	if (target.path == "/api/auth/pk" && req.method() == http::verb::get) {
		if (auto public_key = std::getenv("PUBLIC_KEY")) {
			response.result(http::status::ok);
			response.body() = ("{\"public_key\": \"" + std::string(public_key) + "\"}");
		} else {
			response.result(http::status::not_found);
			response.body() = R"({"error":"Not found Public Key"})";
		}
	}

	return response;
}

void auth_handler::async_handle_request(const request_t &req, socket_t &socket, response_handler on_response) {
	CryptoManager crypto_manager;

	response_t response;
	response.version(req.version());
	response.set(http::field::server, "Shent.User");
	response.set(http::field::content_type, "application/json");

	request_target target(req.target());

	if (target.path == "/api/auth/pk" && req.method() == http::verb::get) {
		if (auto public_key = std::getenv("PUBLIC_KEY")) {
			response.result(http::status::ok);
			response.body() = ("{\"public_key\": \"" + std::string(public_key) + "\"}");
		} else {
			set_bad_response(response, http::status::not_found, "Not found Public Key");
		}

		response.prepare_payload();
		on_response(std::move(response));
		return;
	}

	if (target.path == "/api/auth/login" && req.method() == http::verb::post) {
		std::optional<crypt_data> obj = crypt_data::from_json(req.body());

		if (!obj.has_value()) {
			set_bad_response(response, http::status::bad_request, "Invalid encrypted data");
			response.prepare_payload();
			on_response(std::move(response));
			return;
		}

		std::optional<AuthRequest> req_auth_opt = AuthRequest::from_json(crypto_manager.decrypt(obj->data));
		if (!req_auth_opt.has_value()) {
			set_bad_response(response, http::status::bad_request, "Invalid request structure");
			response.prepare_payload();
			on_response(std::move(response));
			return;
		}

		std::string login = req_auth_opt->login;
		user_repository_->async_get(login, [login, req_auth_opt, response = std::move(response), on_response, this](std::optional<User> user_opt) mutable {
			if (!user_opt.has_value()) {
				set_bad_response(response, http::status::unauthorized, "User not found");
				response.prepare_payload();
				on_response(std::move(response));
				return;
			}

			int user_id = user_opt->id;
			user_repository_->async_get_auth_data(user_id, [user_id, req_auth_opt,
				this, response, on_response](std::optional<UserAuth> auth_data_opt) mutable {

				if (!auth_data_opt.has_value()) {
					set_bad_response(response, http::status::unauthorized, "Auth data not found");
					response.prepare_payload();
					on_response(std::move(response));
					return;
				}

				std::string hashed = CryptoManager::hash_password(req_auth_opt->password, auth_data_opt->salt);
				if (hashed != auth_data_opt->password_hash) {
					set_bad_response(response, http::status::unauthorized, "Incorrect password");
					response.prepare_payload();
					on_response(std::move(response));
					return;
				}


				response_t success_response;
				success_response.result(http::status::ok);
				success_response.set(http::field::server, "Shent.User");
				success_response.set(http::field::content_type, "application/json");

				AuthTokens tokens = AuthTokens::create(user_id, ACCESS_TOKEN_TTL);
				auth_repository_->add_refresh_token(user_id, tokens.refresh_token, REFRESH_TOKEN_TTL);

				json result = tokens;

				success_response.body() = result.dump();
				success_response.prepare_payload();
				on_response(std::move(success_response));
			});

		});
	}

}
