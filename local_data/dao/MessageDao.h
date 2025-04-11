#ifndef MESSAGEDAO_H
#define MESSAGEDAO_H

#include "../model/Message.h"
#include <functional>

#include "../ShentDB.h"


class MessageDao {
	using Callback = std::function<void(std::optional<Message>)>;

	ShentDB& db_;
public:
	explicit MessageDao(ShentDB& db);

	void getById(int64_t id, Callback callback);
};



#endif //MESSAGEDAO_H
