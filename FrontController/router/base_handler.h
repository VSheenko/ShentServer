#ifndef BASEHANDLER_H
#define BASEHANDLER_H

#include <boost/beast/http.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <optional>

class base_handler {
public:
	using response_t = boost::beast::http::response<boost::beast::http::string_body>;
	using request_t = boost::beast::http::request<boost::beast::http::string_body>;
	using socket_t = boost::asio::ip::tcp::socket;
	using response_handler = std::function<void(response_t)>;

	virtual ~base_handler() = default;
	virtual std::optional<response_t> handle_request(const request_t &req, socket_t &socket) = 0;
	virtual void async_handle_request(const request_t& req, socket_t& socket, response_handler on_response);

protected:
	void set_bad_response(response_t& response,
	                      boost::beast::http::status status, std::string message, const response_handler &on_response);
};


inline void base_handler::async_handle_request(const request_t &req, socket_t &socket, response_handler on_response) {
	auto response = handle_request(req, socket);
	if (response.has_value())
		on_response(std::move(response.value()));
}

inline void base_handler::set_bad_response(response_t &response,
                                           const boost::beast::http::status status, std::string message, const response_handler &on_response) {

	response.result(status);
	response.body() = ("{\"error\": \"" + message + "\"}");
	response.prepare_payload();
	on_response(std::move(response));
}

#endif //BASEHANDLER_H
