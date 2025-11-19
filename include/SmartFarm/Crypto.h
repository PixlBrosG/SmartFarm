#pragma once

#include <string>

namespace SmartFarm::Crypto {

	constexpr std::string_view KEY = "smartfarm_secret_key";

	inline std::string XorEncryptDecrypt(const std::string& plainText)
	{
		std::string out = plainText;
		for (size_t i = 0; i < plainText.length(); i++)
			out[i] ^= KEY[i % KEY.length()];
		return out;
	}

}
