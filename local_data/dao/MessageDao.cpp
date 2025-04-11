#include "MessageDao.h"
#include <format>

MessageDao::MessageDao(ShentDB &db) : db_(db) {}



void MessageDao::getById(int64_t id, Callback callback) {
	std::string query = std::format(
		 "SELECT messages.id, messages.sender_id, messages.recipient_id, messages.chat_id, messages.content, attachments.attachments_entity_id\n"
		 "FROM messages\n"
		 "LEFT JOIN attachments ON messages.id = attachments.message_id\n"
		 "WHERE messages.id = {};", id);

	db_.asyncQuery(query, [callback](PGresult* result) {
		if (!result || PQntuples(result) == 0) {
			callback(std::nullopt);
			return;
		}

		callback(Message::fromPGResult(result, 0));
	});
}
