#ifndef WEBSOCKET_HANDLER_H
#define WEBSOCKET_HANDLER_H

#include "SessionManager.h"
#include "../FrontController/router/base_handler.h"


class websocket_handler : public base_handler {
	::std::shared_ptr<SessionManager> manager_;

public:
	websocket_handler();

	std::optional<boost::beast::http::response<boost::beast::http::string_body>> handle_request(
		const boost::beast::http::request<boost::beast::http::string_body> &req,
		boost::asio::ip::tcp::socket &socket) override;
};



#endif //WEBSOCKET_HANDLER_H
