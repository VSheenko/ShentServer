#include "CryptoManager.h"

#include <fstream>
#include <cryptopp/sha.h>
#include <cryptopp/pwdbased.h>
#include <cryptopp/hex.h>
#include <cryptopp/osrng.h>
#include <cryptopp/base64.h>
#include <boost/uuid/uuid_io.hpp>
#include <jwt-cpp/jwt.h>

CryptoManager::CryptoManager() {
	private_key_pem_ = load_pem_key("private_key.pem");
	public_key_pem_ = load_pem_key("public_key.pem");

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


std::string CryptoManager::hash(const std::string &password, const std::string &salt64) {
	std::string salt_decoded;
	CryptoPP::StringSource(salt64, true,
		new CryptoPP::Base64Decoder(
			new CryptoPP::StringSink(salt_decoded)
		)
	);

	CryptoPP::SecByteBlock derived(32);

	CryptoPP::PKCS5_PBKDF2_HMAC<CryptoPP::SHA256> pbkdf2;
	pbkdf2.DeriveKey(
		derived, derived.size(),
		0,
		reinterpret_cast<const CryptoPP::byte*>(password.data()), password.size(),
		reinterpret_cast<const CryptoPP::byte*>(salt_decoded.data()), salt_decoded.size(),
		100000
	);

	std::string encoded;
	CryptoPP::StringSource(derived, derived.size(), true,
	                       new CryptoPP::HexEncoder(new CryptoPP::StringSink(encoded))
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

std::string CryptoManager::load_pem_key(const std::string &filename) {
	std::ifstream file(filename);
	if (!file.is_open()) {
		throw std::runtime_error("Failed to open file: " + filename);
	}

	std::stringstream buffer;
	buffer << file.rdbuf();
	file.close();
	return buffer.str();
}

std::string CryptoManager::get_private_key_pem() {
	return std::getenv("PRIVATE_KEY");
}

std::string CryptoManager::get_public_key_pem() {
	return std::getenv("PUBLIC_KEY");
}

std::string CryptoManager::salt64_generate(size_t length) {
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

std::string CryptoManager::uuid_generate() {
	return to_string(uuid_generator_());
}

std::string CryptoManager::jwt_generate(const std::string &type, const std::string& issuer, int user_id, int ttl_sec) {
	return jwt::create().set_type(type)
		.set_subject(std::to_string(user_id))
		.set_issuer(issuer)
		.set_expires_in(std::chrono::seconds(ttl_sec))
		.sign(jwt::algorithm::rs256("", private_key_pem_, "", ""));
}

int CryptoManager::jwt_validate(const std::string &token, const std::string &issuer) {
	try {
		auto decoded = jwt::decode(token);

		auto verifier = jwt::verify()
				.allow_algorithm(jwt::algorithm::rs256(public_key_pem_, "", "", ""))
				.with_issuer(issuer).leeway(60);

		verifier.verify(decoded);

		int user_id = std::stoi(decoded.get_subject());
		return user_id;
	} catch (...) {
		return -1;
	}
}
