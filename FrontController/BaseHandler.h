#ifndef BASEHANDLER_H
#define BASEHANDLER_H

#include <boost/beast/http.hpp>

class BaseHandler {
public:
	virtual ~BaseHandler() = default;
	virtual boost::beast::http::response<boost::beast::http::string_body> handle_request(
		const boost::beast::http::request<boost::beast::http::string_body>& req) = 0;
};

#endif //BASEHANDLER_H
