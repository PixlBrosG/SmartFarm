#include "SmartFarm/Message.h"

namespace SmartFarm {

	std::string Message::Serialize() const
	{
		nlohmann::json json;
		json["type"] = Protocol::ToString(Type);
		json["payload"] = Payload;
		return json.dump();
	}

	Message Message::Deserialize(const std::string& data)
	{
		auto json = nlohmann::json::parse(data);
		Message msg;

		std::string type = json.at("type");
		msg.Type = Protocol::MessageTypeFromString(type);
		msg.Payload = json.at("payload");
		return msg;
	}

}
