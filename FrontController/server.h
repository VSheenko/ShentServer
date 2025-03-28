#ifndef SERVER_H
#define SERVER_H

#include "session.h"

class server {
    boost::asio::io_context io_context_;
    boost::asio::ip::tcp::acceptor acceptor_;
    std::vector<std::thread> workers_;

public:
    server(short port, int thread_count);
    ~server();

private:
    void accept_connections();


};



#endif //SERVER_H
