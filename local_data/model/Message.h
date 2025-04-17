#ifndef MESSAGE_H
#define MESSAGE_H
#include <libpq-fe.h>
#include <optional>
#include <string>
#include <nlohmann/json.hpp>


struct Message {
	int64_t id;
	int sender_id;
	int recipient_id;
	int64_t chat_id;
	std::string text;
	bool attachmentExists;

	static std::optional<Message> fromPGResult(PGresult* result, int row);

	nlohmann::json to_json();
	static std::optional<Message> from_json(nlohmann::json &j);
};



#endif //MESSAGE_H
