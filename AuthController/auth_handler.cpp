#include "auth_handler.h"
#include <cstdlib>
#include <iostream>
#include <nlohmann/json.hpp>
#include <boost/url.hpp>

#include "../Crypto/CryptoManager.h"
#include "../local_data/PostgresClient.h"
#include "../local_data/model/request_target.h"
#include "../local_data/model/User.h"
#include "model/AuthRequest.h"
#include "model/crypt_data.h"
#include "model/RegisterRequest.h"


namespace http = boost::beast::http;
using json = nlohmann::json;

auth_handler::auth_handler(std::shared_ptr<UserRepository> user_repository, std::shared_ptr<AuthRepository> auth_repository): user_repository_(
	std::move(user_repository)), auth_repository_(auth_repository) {}

std::optional<http::response<http::string_body>> auth_handler::handle_request(
	const http::request<http::string_body> &req, boost::asio::ip::tcp::socket &socket) { }

void auth_handler::async_handle_request(const request_t &req, socket_t &socket, response_handler on_response) {
	CryptoManager crypto_manager;

	response_t response;
	response.version(req.version());
	response.set(http::field::server, "Shent.Auth");
	response.set(http::field::content_type, "application/json");


	request_target target(req.target());

	std::string user_agent;
	if (req.count(http::field::user_agent)) {
		user_agent = req[http::field::user_agent];
	}


	if (target.path == "/api/auth/pk" && req.method() == http::verb::get) {
		if (auto public_key = std::getenv("PUBLIC_KEY")) {
			response.result(http::status::ok);
			response.body() = ("{\"public_key\": \"" + std::string(public_key) + "\"}");
		} else {
			set_bad_response(response, http::status::not_found, "Not found Public Key", on_response);
		}

		response.prepare_payload();
		on_response(std::move(response));
		return;
	}

	if (target.path == "/api/auth/register" && req.method() == http::verb::post) {
		std::optional<crypt_data> obj = crypt_data::from_json(req.body());

		if (!obj.has_value()) {
			set_bad_response(response, http::status::bad_request, "Invalid encrypted data", on_response);
			return;
		}

		std::optional<RegisterRequest> register_request_opt = RegisterRequest::from_json(crypto_manager.decrypt(obj->data));
		if (!register_request_opt.has_value()) {
			set_bad_response(response, http::status::bad_request, "Invalid or malformed registration request data", on_response);
			return;
		}

		RegisterRequest register_request = register_request_opt.value();
		user_repository_->async_get(register_request.login,
			[this, register_request, response, on_response](std::optional<User> user_opt) mutable {
			if (user_opt.has_value()) {
				set_bad_response(response, http::status::conflict, "User with this login already exists", on_response);
				return;
			}

			registration(register_request, response, on_response);
		});
	}

	if (target.path == "/api/auth/salt" && req.method() == http::verb::get) {
		std::unordered_map<std::string, std::string> params = get_params(req);

		if (!params.contains("login")) {
			set_bad_response(response, http::status::bad_request, "Missing required parameter: login", on_response);
			return;
		}

		get_salt(params["login"], on_response);
	}

	if (target.path == "/api/auth/login" && req.method() == http::verb::post) {
		std::optional<crypt_data> obj = crypt_data::from_json(req.body());

		if (!obj.has_value()) {
			set_bad_response(response, http::status::bad_request, "Invalid encrypted data", on_response);
			return;
		}

		std::optional<AuthRequest> req_auth_opt = AuthRequest::from_json(crypto_manager.decrypt(obj->data));
		if (!req_auth_opt.has_value()) {
			set_bad_response(response, http::status::bad_request, "Invalid request structure", on_response);
			return;
		}

		RequestProp prop;
		prop.version = req.version();
		prop.user_agent = user_agent;

		login(*req_auth_opt, prop, on_response);
	}

}

void auth_handler::login(const AuthRequest &auth_data, const RequestProp& req_prop, const response_handler &on_response) {
	response_t response;
	response.version(req_prop.version);
	response.set(http::field::server, "Shent.Auth");
	response.set(http::field::content_type, "application/json");

	user_repository_->async_get(auth_data.login, [this, response, auth_data,
		req_prop, on_response](std::optional<User> user_opt) mutable {
		if (!user_opt.has_value()) {
			set_bad_response(response, http::status::unauthorized, "User not found", on_response);
			return;
		}

		user_repository_->async_get_auth_data(user_opt->id, [this, auth_data, user_opt, req_prop,
			response, on_response](std::optional<UserAuth> confirm_auth_data_opt) mutable {
			if (!confirm_auth_data_opt.has_value()) {
				set_bad_response(response, http::status::unauthorized, "Auth data not found", on_response);
				return;
			}

			if (auth_data.password != confirm_auth_data_opt->password_hash) {
				set_bad_response(response, http::status::unauthorized, "Incorrect password", on_response);
				return;
			}

			response.result(http::status::ok);;

			AuthTokens tokens = get_auth_tokens(user_opt->id,  auth_data.device_id, req_prop.user_agent);
			json result = tokens;

			response.body() = result.dump();
			response.prepare_payload();
			on_response(std::move(response));
		});
	});

}

void auth_handler::registration(const RegisterRequest &registration_data, response_t &response, const response_handler &on_response) {
	User user (registration_data.login, registration_data.login);
	UserAuth user_auth (0, registration_data.password, registration_data.salt, registration_data.private_key);
	user_repository_->async_create_user(user, user_auth, [this, response, on_response](int id) mutable {
		if (id == -1) {
			set_bad_response(response, http::status::internal_server_error,
				"Failed to save data. Please try again later", on_response);
			return;
		}

		response.result(http::status::created);
		response.prepare_payload();
		on_response(response);
	});
}

void auth_handler::get_salt(std::string login, response_handler &on_response) {
	response_t response;
	response.set(http::field::server, "Shent.Auth");
	response.set(http::field::content_type, "application/json");

	user_repository_->async_get_salt(login, [this, response, on_response](std::optional<std::string> salt_opt) mutable {
		if (!salt_opt.has_value()) {
			set_bad_response(response, http::status::not_found, "Salt not found", on_response);
			return;
		}

		response.result(http::status::ok);
		response.body() = ("{\"salt\": \"" + *salt_opt + "\"}");
		response.prepare_payload();
		on_response(response);
	});
}


AuthTokens auth_handler::get_auth_tokens(int user_id, const std::string &device_id, const std::string &user_agent) {
	AuthTokens tokens = AuthTokens::create(user_id, ACCESS_TOKEN_TTL);
	auth_repository_->add_refresh_token(
		user_id,
		tokens.create_refresh_sheet(device_id, user_agent),
		REFRESH_TOKEN_TTL);

	return tokens;
}
