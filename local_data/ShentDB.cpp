#include "ShentDB.h"
#include <iostream>

ShentDB::ShentDB(boost::asio::io_context &io, const std::string &connStr)
	: io_(io),  workGuard_(boost::asio::make_work_guard(io)) {
	connection_ = PQconnectdb(connStr.c_str());

	if (PQstatus(connection_) != CONNECTION_OK)
		throw std::runtime_error(PQerrorMessage(connection_));

	PQsetnonblocking(connection_, 1);
}

ShentDB::~ShentDB() {
	if (connection_)
		PQfinish(connection_);
}


void ShentDB::asyncQuery(const std::string &q, Callback callback) {
	boost::asio::post(io_, [this, q, callback]() {
		PGresult* result = PQexec(connection_, q.c_str());
		callback(result);
		PQclear(result);
	});
}
