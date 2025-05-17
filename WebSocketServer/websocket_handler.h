#ifndef WEBSOCKET_HANDLER_H
#define WEBSOCKET_HANDLER_H

#include "SessionManager.h"
#include "../FrontController/router/base_handler.h"
#include "../FrontController/router/protect_handler.hpp"


class websocket_handler : public base_handler, protect_handler {
	::std::shared_ptr<SessionManager> manager_;
	using base_handler::request_t;
	using base_handler::response_t;

public:
	websocket_handler(std::shared_ptr<CryptoManager> crypto_manager);

	std::optional<response_t> handle_request(const request_t &req, socket_t &socket) override;

	void async_handle_request(const request_t &req, socket_t &socket, response_handler on_response) override;
};



#endif //WEBSOCKET_HANDLER_H
