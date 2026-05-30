#ifndef POSTGRES_CLIENT_H
#define POSTGRES_CLIENT_H

#include <libpq-fe.h>
#include <boost/asio.hpp>


class PostgresClient : public std::enable_shared_from_this<PostgresClient> {
	using Callback = std::function<void(PGresult*)>;

public:
	PostgresClient() = delete;
	explicit PostgresClient(boost::asio::io_context& io, const std::string& connStr, size_t pool_size);
	~PostgresClient();

	void asyncQuery(const std::string& q, std::function<void(PGresult*)> callback);

private:
	std::vector<PGconn*> connections_;
	std::atomic<size_t> current_connection_;
	boost::asio::io_context& io_;
	boost::asio::executor_work_guard<boost::asio::io_context::executor_type> workGuard_;
};



#endif //POSTGRES_CLIENT_H
