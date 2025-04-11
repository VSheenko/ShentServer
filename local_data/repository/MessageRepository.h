#ifndef MESSAGEREPOSITORY_H
#define MESSAGEREPOSITORY_H

#include "../ShentDB.h"
#include "../dao/MessageDao.h"


class MessageRepository {
	std::shared_ptr<MessageDao> dao_;

public:
	explicit MessageRepository(std::shared_ptr<ShentDB> db);

	void async_get(int64_t id, std::function<void(std::optional<Message>)> callback);
};



#endif //MESSAGEREPOSITORY_H
