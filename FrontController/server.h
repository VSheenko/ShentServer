#ifndef SERVER_H
#define SERVER_H

#include "session.h"
#include "router/router.h"

class server {
    boost::asio::io_context io_context_;
    boost::asio::ip::tcp::acceptor acceptor_;
    std::vector<std::thread> workers_;

    boost::asio::executor_work_guard<boost::asio::io_context::executor_type> work_guard_;
    router router_;

public:
    server(short port, int thread_count, const router &rt);
    ~server();

    void run();
private:
    void accept_connections();


};



#endif //SERVER_H
