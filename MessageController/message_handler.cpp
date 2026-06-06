//
// Created by vovan on 24.05.2025.
//

#include "message_handler.h"

message_handler::message_handler(const std::shared_ptr<CryptoManager> &crypto_manager)
	: protect_handler(crypto_manager)
{
}


std::optional<base_handler::response_t> message_handler::handle_request(const request_t &req, socket_t &socket) {}

void message_handler::async_handle_request(const request_t &req, socket_t &socket, response_handler on_response)
{
	base_handler::async_handle_request(req, socket, on_response);
}
