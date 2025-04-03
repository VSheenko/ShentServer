#ifndef SHENTDB_H
#define SHENTDB_H

#include <libpq-fe.h>
#include <boost/asio.hpp>


class ShentDB {
	using Callback = std::function<void(PGresult*)>;

public:
	ShentDB() = delete;
	explicit ShentDB(boost::asio::io_context& io, const std::string& connStr);
	~ShentDB();

	void asyncQuery(const std::string& q, std::function<void(PGresult*)> callback);

private:
	PGconn* connection_;
	boost::asio::io_context& io_;
	boost::asio::executor_work_guard<boost::asio::io_context::executor_type> workGuard_;
};



#endif //SHENTDB_H
