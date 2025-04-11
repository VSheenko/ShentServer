#ifndef MESSAGE_H
#define MESSAGE_H
#include <libpq-fe.h>
#include <optional>
#include <string>


struct Message {
	int64_t id;
	int sender_id;
	int recipient_id;
	int64_t chat_id;
	std::string text;
	bool attachmentExists;

	static std::optional<Message> fromPGResult(PGresult* result, int row);
};



#endif //MESSAGE_H
