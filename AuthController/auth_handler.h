#ifndef AUTH_HANDLER_H
#define AUTH_HANDLER_H
#include "../FrontController/router/base_handler.h"
#include "../local_data/repository/UserRepository.h"
#include "model/AuthRequest.h"
#include "model/crypt_data.h"


class auth_handler : public base_handler {
	std::shared_ptr<UserRepository> user_repository_;

public:
	auth_handler(std::shared_ptr<UserRepository> user_repository);
	std::optional<boost::beast::http::response<boost::beast::http::string_body>> handle_request(
		const request_t &req, socket_t &socket) override;

	inline void async_handle_request(const request_t &req, socket_t &socket, response_handler on_response) override;

	bool verify_password(int user_id, std::string password);
private:
	void get_auth_response(int user_id);
	bool verify_auth_data(AuthRequest data);


};



#endif //AUTH_HANDLER_H
