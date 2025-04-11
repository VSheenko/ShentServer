#ifndef SHENTDB_H
#define SHENTDB_H

#include <libpq-fe.h>
#include <boost/asio.hpp>


class ShentDB : public std::enable_shared_from_this<ShentDB> {
	using Callback = std::function<void(PGresult*)>;

public:
	ShentDB() = delete;
	explicit ShentDB(boost::asio::io_context& io, const std::string& connStr, size_t pool_size);
	~ShentDB();

	void asyncQuery(const std::string& q, std::function<void(PGresult*)> callback);

private:
	std::vector<PGconn*> connections_;
	std::atomic<size_t> current_connection_;
	boost::asio::io_context& io_;
	boost::asio::executor_work_guard<boost::asio::io_context::executor_type> workGuard_;
};



#endif //SHENTDB_H
