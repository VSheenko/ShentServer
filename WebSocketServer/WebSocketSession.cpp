#include "WebSocketSession.h"

#include <iostream>
#include <nlohmann/json.hpp>

#include "../local_data/model/request_target.h"

namespace asio = boost::asio;
namespace beast = boost::beast;
namespace http = beast::http;
using json = nlohmann::json;
using tcp = asio::ip::tcp;

WebSocketSession::WebSocketSession(tcp::socket &&socket, std::shared_ptr<SessionManager> manager):
	ws_(std::move(socket)), manager_(std::move(manager)) {}

WebSocketSession::~WebSocketSession() {
	manager_->remove(user_id_);
}

void WebSocketSession::run(const http::request<http::string_body>& req, int user_id) {
	request_target target(req.target());
	user_id_ = user_id;

	ws_.async_accept(req, [self = shared_from_this()](beast::error_code ec) {
		if (!ec) {
			std::cout << "[WebSocketSession::run]: user_id=" << self->user_id_ << " accepted" << std::endl;
			self->manager_->add(self->user_id_, self);
			self->read();
		}
	});
}

void WebSocketSession::send(Message &msg) {
	ws_.text(true);
	ws_.async_write(boost::asio::buffer(msg.to_json().dump()), [](beast::error_code, std::size_t) {});
}

void WebSocketSession::read() {
	ws_.async_read(buffer_, [self = shared_from_this()](beast::error_code ec, std::size_t) {
		if (ec) {
			std::cerr << "WebSocket closed or error [WebSocketSession::read]: " << ec.message() << std::endl;

			beast::error_code close_ec;
			self->ws_.close(beast::websocket::close_code::normal, close_ec);
			return;
		}

		const std::string s_msg = beast::buffers_to_string(self->buffer_.data());
		self->buffer_.consume(self->buffer_.size());

		try {
			self->handle_incoming(s_msg);
		} catch (std::exception& e) {
			std::cerr << "Error [WebSocketSession::read method:handle_incoming]: " << e.what() << std::endl;
		}

		self->read();
	});
}

void WebSocketSession::write(const std::string& msg) {
	ws_.text(true);
	ws_.async_write(beast::net::buffer(msg), [self = shared_from_this()](beast::error_code ec, std::size_t) mutable {
		if (!ec) self->read();
	});
}

void WebSocketSession::handle_incoming(const std::string &json_str) {
	std::cout << "Message received" << std::endl;

	json j = json::parse(json_str);
	std::optional<Message> msg = Message::from_json(j);

	if (msg == std::nullopt)
		throw std::runtime_error("Error Message parse");

	if (msg->recipient_id == 0) {
		int id = msg->sender_id;
		msg->sender_id = msg->recipient_id;
		msg->recipient_id = msg->id;
		msg->timestamp += 1;

		send(*msg);
		return;
	}

	manager_->get(msg.value().recipient_id)->send(msg.value());
}
