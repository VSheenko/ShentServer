#ifndef SHENTDB_H
#define SHENTDB_H
#include <memory>

#include "PostgresClient.h"
#include "RedisClient.h"


class ShentDB {
	std::shared_ptr<PostgresClient> pq_;
	std::shared_ptr<RedisClient> redis_;

public:
	ShentDB(const std::shared_ptr<PostgresClient> &pq, const std::shared_ptr<RedisClient> &redis);

	std::shared_ptr<PostgresClient> pq();
	std::shared_ptr<RedisClient> redis();

};



#endif //SHENTDB_H
