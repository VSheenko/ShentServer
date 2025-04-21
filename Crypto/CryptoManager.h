#ifndef CRYPTOMANAGER_H
#define CRYPTOMANAGER_H

#include <string>
#include <cstdlib>
#include <stdexcept>
#include <vector>
#include <cryptopp/rsa.h>
#include <cryptopp/osrng.h>
#include <cryptopp/base64.h>
#include <cryptopp/osrng.h>

class CryptoManager {
public:
	CryptoManager();
	~CryptoManager();

	std::string encrypt(const std::string& plaintext);
	std::string decrypt(const std::string& ciphertext);

	static std::string hash_password(const std::string& password, const std::string& salt64);
	static std::string generate_salt64(size_t length = 16);

private:
	CryptoPP::RSA::PublicKey public_key_;
	CryptoPP::RSA::PrivateKey private_key_;

	void load_key(const std::string& env_var_name);
};

#endif //CRYPTOMANAGER_H
