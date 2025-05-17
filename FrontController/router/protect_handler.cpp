#include "protect_handler.hpp"

#include <iostream>

protect_handler::protect_handler(std::shared_ptr<CryptoManager> crypto_manager) {
	crypto_manager_ = crypto_manager;
	issuer_ = "ShentServer";
}

int protect_handler::authorize(const request_t &req) {
	std::string token;

	if (!extract_token(req, token))
		return -1;

	int user_id = is_token_valid(token);
	if (user_id != -1)
		std::cout << "[protect_handler::authorize]: User id=" << user_id << " authorized" << std::endl;

	return user_id;
}

int protect_handler::is_token_valid(const std::string &token) {
	int res = crypto_manager_->jwt_validate(token, issuer_);
	return res;
}

bool protect_handler::extract_token(const request_t & req, std::string & out_token) {
	auto auth = req[boost::beast::http::field::authorization];
	const std::string prefix = "Bearer ";
	if (auth.substr(0, prefix.size()) == prefix) {
		out_token = auth.substr(prefix.size());
		return true;
	}
	return false;
}
