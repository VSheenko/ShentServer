#include "Message.h"

std::optional<Message> Message::fromPGResult(PGresult *result, int row) {
	if (!result || PQntuples(result) <= row)
		return std::nullopt;

	int64_t id = std::stoll(PQgetvalue(result, row, 0));
	int sender = std::stoi(PQgetvalue(result, row, 1));
	int recipient = PQgetisnull(result, row, 2) ? -1 : std::stoi(PQgetvalue(result, row, 2));
	int64_t chat = PQgetisnull(result, row, 3) ? -1 : std::stoll(PQgetvalue(result, row, 3));
	std::string text = PQgetisnull(result, row, 4) ? "" : PQgetvalue(result, row, 4);
	bool is = PQgetisnull(result, row, 5) ? false : true;

	return Message {
		id, sender, recipient, chat, text, is
	};
}

nlohmann::json Message::to_json() {
	nlohmann::json j = {
		{"id", id},
		{"sender_id", sender_id},
		{"recipient_id", recipient_id},
		{"chat_id", chat_id},
		{"text", text},
		{"attachmentExists", attachmentExists},
	};

	return j;
}

std::optional<Message> Message::from_json(nlohmann::json &j) {
	Message msg;

	j.at("id").get_to(msg.id);
	j.at("sender_id").get_to(msg.sender_id);
	j.at("recipient_id").get_to(msg.recipient_id);
	j.at("chat_id").get_to(msg.chat_id);
	j.at("text").get_to(msg.text);
	j.at("attachmentExists").get_to(msg.attachmentExists);

	return msg;
}

