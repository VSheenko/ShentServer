#include "PostgresClient.h"

PostgresClient::PostgresClient(boost::asio::io_context &io, const std::string &connStr, size_t pool_size)
	: io_(io),  workGuard_(boost::asio::make_work_guard(io)) {

	for (size_t i = 0; i < pool_size; i++) {
		PGconn* conn = PQconnectdb(connStr.c_str());

		if (PQstatus(conn) != CONNECTION_OK)
			throw std::runtime_error(PQerrorMessage(conn));

		PQsetnonblocking(conn, 1);
		connections_.push_back(conn);
	}
}

PostgresClient::~PostgresClient() {
	for (PGconn* conn : connections_) {
		if (conn)
			PQfinish(conn);
	}
}


void PostgresClient::asyncQuery(const std::string &q, Callback callback) {
	auto self = shared_from_this();

	size_t conn_ind = current_connection_++ % connections_.size();

	boost::asio::post(io_, [self, q, callback, conn_ind]() {
		PGresult* result = PQexec(self->connections_[conn_ind], q.c_str());
		callback(result);
		PQclear(result);

	});
}

