#ifndef SESSIONMANAGER_H
#define SESSIONMANAGER_H

#include <memory>
#include <unordered_map>
#include <vector>

class WebSocketSession;

class SessionManager {
	std::unordered_map<int, std::weak_ptr<WebSocketSession>> sessions_;
	std::mutex mutex_;

public:
	void add(int user_id, std::shared_ptr<WebSocketSession> ptr);
	void remove(int user_id);
	std::shared_ptr<WebSocketSession> get(int user_id);

	std::pmr::vector<int> get_users();
};



#endif //SESSIONMANAGER_H
