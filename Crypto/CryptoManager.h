#ifndef CRYPTOMANAGER_H
#define CRYPTOMANAGER_H

#include <string>
#include <cryptopp/rsa.h>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>

class CryptoManager {
public:
	CryptoManager();
	~CryptoManager();

	std::string encrypt(const std::string& plaintext);
	std::string decrypt(const std::string& ciphertext);

	static std::string hash(const std::string& password, const std::string& salt64);
	static std::string salt64_generate(size_t length = 16);

	std::string uuid_generate();

	std::string jwt_generate(const std::string& type, const std::string& issuer,
	                         int user_id, int ttl_sec = 0);
	int jwt_validate(const std::string& token, const std::string &issuer);

private:
	CryptoPP::RSA::PublicKey public_key_;
	CryptoPP::RSA::PrivateKey private_key_;

	std::string public_key_pem_;
	std::string private_key_pem_;

	boost::uuids::random_generator uuid_generator_;

	void load_key(const std::string& env_var_name);
	std::string load_pem_key(const std::string& filename);

	std::string get_private_key_pem();
	std::string get_public_key_pem();
};

#endif //CRYPTOMANAGER_H
