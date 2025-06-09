#include "ShentDB.h"

ShentDB::ShentDB(const std::shared_ptr<PostgresClient> &pq, const std::shared_ptr<RedisClient> &redis)
	: pq_(pq), redis_(redis) {}

std::shared_ptr<PostgresClient> ShentDB::pq() {
	return pq_;
}

std::shared_ptr<RedisClient> ShentDB::redis() {
	return redis_;
}
