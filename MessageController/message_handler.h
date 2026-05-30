#ifndef MESSAGE_HANDLER_H
#define MESSAGE_HANDLER_H
#include "../FrontController/router/base_handler.h"
#include "../FrontController/router/protect_handler.hpp"


class message_handler final : public base_handler, public protect_handler{
	using request_t = base_handler::request_t;
	using  response_t = base_handler::response_t;

public:
	explicit message_handler(const std::shared_ptr<CryptoManager> &crypto_manager);

	std::optional<response_t> handle_request(const request_t &req, socket_t &socket) override;

	void async_handle_request(const request_t &req, socket_t &socket, response_handler on_response) override;
};



#endif //MESSAGE_HANDLER_H
