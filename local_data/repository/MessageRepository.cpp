#include "MessageRepository.h"

MessageRepository::MessageRepository(std::shared_ptr<PostgresClient> db) {
	dao_ = std::make_shared<MessageDao>(*db);
}

void MessageRepository::async_get(int64_t id, std::function<void(std::optional<Message>)> callback) {
	dao_->getById(id, [callback](std::optional<Message> message) {
		callback(message);
	} );
}
