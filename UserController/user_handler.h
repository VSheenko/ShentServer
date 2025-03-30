#ifndef USER_HANDLER_H
#define USER_HANDLER_H

#include "../FrontController/router/base_handler.h"

class user_handler : public base_handler {
public:
	boost::beast::http::response<boost::beast::http::string_body> handle_request(
		const boost::beast::http::request<boost::beast::http::string_body> &req) override;
};



#endif //USER_HANDLER_H
