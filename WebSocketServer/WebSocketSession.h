#ifndef WEBSOCKETSESSION_H
#define WEBSOCKETSESSION_H


#include "SessionManager.h"

#include "../local_data/model/Message.h"

#include <memory>
#include <boost/beast.hpp>
#include <boost/asio.hpp>

class WebSocketSession : public std::enable_shared_from_this<WebSocketSession> {
	boost::beast::websocket::stream<boost::beast::tcp_stream> ws_;
	boost::beast::flat_buffer buffer_;
	std::shared_ptr<SessionManager> manager_;
	int user_id_ = -1;

public:
	explicit WebSocketSession(boost::asio::ip::tcp::socket&& socket, std::shared_ptr<SessionManager>);
	~WebSocketSession();

	void run(const boost::beast::http::request<boost::beast::http::string_body>& req, int user_id);
	void send(Message& msg);


private:
	void read();
	void write(const std::string& msg);

	void handle_incoming(const std::string& json_str);
};



#endif //WEBSOCKETSESSION_H
