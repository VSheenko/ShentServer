#include "Message.h"
#include <iostream>

std::optional<Message> Message::fromPGResult(PGresult *result, int row) {
	if (!result || PQntuples(result) <= row)
		return std::nullopt;

	int64_t id = std::stoll(PQgetvalue(result, row, 0));
	int sender = std::stoi(PQgetvalue(result, row, 1));
	int recipient = PQgetisnull(result, row, 2) ? -1 : std::stoi(PQgetvalue(result, row, 2));
	int64_t chat = PQgetisnull(result, row, 3) ? -1 : std::stoll(PQgetvalue(result, row, 3));
	std::string content = PQgetisnull(result, row, 4) ? "" : PQgetvalue(result, row, 4);
	bool is = PQgetisnull(result, row, 5) ? false : true;

	return Message {
		id, sender, recipient, chat, content, is
	};
}

// TODO: Нормальную структуру сообщений
nlohmann::json Message::to_json() {
	nlohmann::json j = {
		{"id", id},
		{"sender_id", sender_id},
		{"recipient_id", recipient_id},
		{"chat_id", -1},
			{"timestamp", timestamp},
		{"content", content},
		{"attachmentExists", attachmentExists},
	};

	return j;
}

// TODO: Нормальную структуру сообщений
std::optional<Message> Message::from_json(nlohmann::json &j) {
	Message msg;

	std::cout << j.dump() << std::endl;

	j.at("id").get_to(msg.id);
	j.at("sender_id").get_to(msg.sender_id);
	j.at("recipient_id").get_to(msg.recipient_id);
	j.at("timestamp").get_to(msg.timestamp);
	msg.chat_id = -1;
	j.at("content").get_to(msg.content);
	// j.at("attachmentExists").get_to(msg.attachmentExists);

	return msg;
}

