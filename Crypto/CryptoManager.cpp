#include "CryptoManager.h"
#include <cryptopp/sha.h>
#include <cryptopp/pwdbased.h>
#include <cryptopp/hex.h>
#include <vector>

CryptoManager::CryptoManager() {
	std::string s =std::getenv("PUBLIC_KEY");
	if (s.empty())
		throw std::runtime_error("PUBLIC_KEY environment variable is not set.");

	s = std::getenv("PRIVATE_KEY");
	if (s.empty())
		throw std::runtime_error("PRIVATE_KEY environment variable is not set.");

	load_key("PUBLIC_KEY");
	load_key("PRIVATE_KEY");
}

CryptoManager::~CryptoManager() {

}

std::string CryptoManager::encrypt(const std::string &plaintext) {
	std::string encrypted;

	CryptoPP::AutoSeededRandomPool rng;
	CryptoPP::RSAES_PKCS1v15_Encryptor encryptor(public_key_);
	CryptoPP::StringSource(plaintext, true,
	                       new CryptoPP::PK_EncryptorFilter(rng, encryptor,
	                                                        new CryptoPP::StringSink(encrypted)
	                       )
	);

	return encrypted;
}

std::string CryptoManager::decrypt(const std::string &ciphertext) {
	try {
		std::string decoded;
		CryptoPP::StringSource(ciphertext, true,
			new CryptoPP::Base64Decoder(
				new CryptoPP::StringSink(decoded)
			)
		);

		std::string decrypted;
		CryptoPP::AutoSeededRandomPool rng;
		CryptoPP::RSAES_PKCS1v15_Decryptor decryptor(private_key_);

		CryptoPP::StringSource ss(decoded, true,
			new CryptoPP::PK_DecryptorFilter(rng, decryptor,
				new CryptoPP::StringSink(decrypted)
			)
		);

		return decrypted;
	} catch (const CryptoPP::Exception &e) {
		throw std::runtime_error("Decryption failed: " + std::string(e.what()));
	}
}


std::string CryptoManager::hash_password(const std::string &password, const std::string &salt64) {
	using namespace CryptoPP;


	std::string salt_decoded;
	StringSource(salt64, true,
		new Base64Decoder(
			new StringSink(salt_decoded)
		)
	);

	SecByteBlock derived(32);

	PKCS5_PBKDF2_HMAC<SHA256> pbkdf2;
	pbkdf2.DeriveKey(
		derived, derived.size(),
		0,
		reinterpret_cast<const byte*>(password.data()), password.size(),
		reinterpret_cast<const byte*>(salt_decoded.data()), salt_decoded.size(),
		100000
	);

	std::string encoded;
	StringSource(derived, derived.size(), true,
		new HexEncoder(new StringSink(encoded))
	);

	return encoded;
}

void CryptoManager::load_key(const std::string& env_var_name) {
	const char* keyBase64 = std::getenv(env_var_name.c_str());
	if (!keyBase64) {
		throw std::runtime_error("Environment variable not found: " + env_var_name);
	}

	std::string decoded;
	CryptoPP::StringSource ss(keyBase64, true,
	                          new CryptoPP::Base64Decoder(
		                          new CryptoPP::StringSink(decoded)
	                          )
	);

	CryptoPP::StringSource ss2(decoded, true);
	if (env_var_name == "PUBLIC_KEY")
		public_key_.Load(ss2);
	else if (env_var_name == "PRIVATE_KEY")
		private_key_.Load(ss2);
}

std::string CryptoManager::generate_salt64(size_t length) {
	using namespace CryptoPP;

	AutoSeededRandomPool rng;
	SecByteBlock salt(length);
	rng.GenerateBlock(salt, salt.size());

	std::string encoded;
	StringSource(salt, salt.size(), true,
		new Base64Encoder(new StringSink(encoded), false)
	);

	return encoded;
}
