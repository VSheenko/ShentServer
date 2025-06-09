#include "SessionManager.h"
#include "WebSocketSession.h"

void SessionManager::add(int user_id, std::shared_ptr<WebSocketSession> ptr) {
	std::lock_guard lock(mutex_);
	sessions_[user_id] = ptr;
}

void SessionManager::remove(int user_id) {
	std::lock_guard lock(mutex_);
	sessions_.erase(user_id);
}

std::shared_ptr<WebSocketSession> SessionManager::get(int user_id) {
	std::lock_guard lock(mutex_);
	auto it = sessions_.find(user_id);
	if (it != sessions_.end()) {
		if (auto session = it->second.lock())
			return session;
		sessions_.erase(it);
	}
	return nullptr;
}

std::pmr::vector<int> SessionManager::get_users() {
	std::pmr::vector<int> res;

	for (auto& [key, value] : sessions_) {
		res.push_back(key);
	}

	return res;
}
