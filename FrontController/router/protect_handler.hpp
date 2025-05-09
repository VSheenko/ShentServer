#ifndef PROTECT_BASE_HANDLER_H
#define PROTECT_BASE_HANDLER_H

#include <string>
#include <boost/beast.hpp>

#include "../../Crypto/CryptoManager.h"

class protect_handler {
private:
	using request_t = boost::beast::http::request<boost::beast::http::string_body>;
	using response_t = boost::beast::http::response<boost::beast::http::string_body>;



public:
	explicit protect_handler(std::shared_ptr<CryptoManager> crypto_manager, const std::string& issuer);

	int authorize (const request_t& req);

protected:
	std::shared_ptr<CryptoManager> crypto_manager_;
	std::string issuer_;

	int is_token_valid(const std::string& token);
	bool extract_token(const request_t & req, std::string & token);

	~protect_handler() = default;
};

#endif //PROTECT_BASE_HANDLER_H
