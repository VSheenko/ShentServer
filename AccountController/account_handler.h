#ifndef ACCOUNT_HANDLER_H
#define ACCOUNT_HANDLER_H

#include "../FrontController/router/base_handler.h"
#include "../FrontController/router/protect_handler.hpp"
#include <optional>


class account_handler : public base_handler, protect_handler {
	using base_handler::request_t;
	using base_handler::response_t;

public:
	explicit account_handler(const std::shared_ptr<CryptoManager> &crypto_manager);

	std::optional<response_t> handle_request(const request_t &req, socket_t &socket) override;

	void async_handle_request(const request_t &req, socket_t &socket, response_handler on_response) override;
};



#endif //ACCOUNT_HANDLER_H
