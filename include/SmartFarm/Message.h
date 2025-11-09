#pragma once

#include <string>
#include <nlohmann/json.hpp>

#include "Protocol.h"

namespace SmartFarm {

	struct Message
	{
		Protocol::MessageType Type;
		nlohmann::json Payload;

		std::string Serialize() const;

		static Message Deserialize(const std::string& data);
	};

}