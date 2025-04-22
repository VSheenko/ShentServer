#ifndef MESSAGEDAO_H
#define MESSAGEDAO_H

#include "../model/Message.h"
#include <functional>

#include "../PostgresClient.h"


class MessageDao {
	using Callback = std::function<void(std::optional<Message>)>;

	PostgresClient& db_;
public:
	explicit MessageDao(PostgresClient& db);

	void getById(int64_t id, Callback callback);
};



#endif //MESSAGEDAO_H
