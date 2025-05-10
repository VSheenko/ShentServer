#ifndef AUTH_HANDLER_H
#define AUTH_HANDLER_H
#include "../FrontController/router/base_handler.h"
#include "../local_data/repository/AuthRepository.h"
#include "../local_data/repository/UserRepository.h"
#include "model/AuthTokens.hpp"
#include "model/AuthRequest.h"
#include "model/crypt_data.h"
#include "model/RegisterRequest.h"

class auth_handler : public base_handler {
	const int REFRESH_TOKEN_TTL = 2592000;
	const int ACCESS_TOKEN_TTL = 300;

	std::shared_ptr<UserRepository> user_repository_;
	std::shared_ptr<AuthRepository> auth_repository_;

public:
	auth_handler(std::shared_ptr<UserRepository> user_repository, std::shared_ptr<AuthRepository> auth_repository);
	std::optional<boost::beast::http::response<boost::beast::http::string_body>> handle_request(
		const request_t &req, socket_t &socket) override;

	inline void async_handle_request(const request_t &req, socket_t &socket, response_handler on_response) override;
private:
	void registration(const RegisterRequest &registration_data, response_t &response, const response_handler &on_response);
	void get_salt(std::string login, response_handler& on_response);

	AuthTokens get_auth_tokens(int user_id, const std::string& device_id, const std::string& user_agent);

};



#endif //AUTH_HANDLER_H
