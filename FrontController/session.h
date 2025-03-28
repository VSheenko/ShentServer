#ifndef SESSION_H
#define SESSION_H

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <memory>

class session : public std::enable_shared_from_this<session> {
private:
    boost::asio::ip::tcp::socket socket_;
    boost::beast::flat_buffer buffer_;
    boost::beast::http::request<boost::beast::http::string_body> request_;

public:
    explicit session(boost::asio::ip::tcp::socket socket);
    void start();
    virtual ~session() = default;

private:
    void read_request();
    void processes_request();
    void send_response(boost::beast::http::response<boost::beast::http::string_body> response);

    void close_connection();
};



#endif //SESSION_H
