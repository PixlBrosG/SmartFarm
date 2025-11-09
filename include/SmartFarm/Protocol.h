#pragma once

#include <string>
#include <cstdint>

namespace SmartFarm::Protocol {

	constexpr const char* NAME = "SmartFarm Protocol";
	constexpr const char* VERSION = "1.0";
	constexpr uint16_t DEFAULT_PORT = 5000;
	constexpr size_t MAX_PACKET_SIZE = 4096;
	constexpr int HEARTBEAT_INTERVAL = 5;

	enum class MessageType : uint8_t
	{
		HELLO,
		ACK,
		SENSOR_UPDATE,
		ACTUATOR_STATUS,
		COMMAND,
		ERROR
	};

	inline std::string ToString(MessageType type)
	{
		switch (type)
		{
			case MessageType::HELLO: return "HELLO";
			case MessageType::ACK: return "ACK";
			case MessageType::SENSOR_UPDATE: return "SENSOR_UPDATE";
			case MessageType::ACTUATOR_STATUS: return "ACTUATOR_STATUS";
			case MessageType::COMMAND: return "COMMAND";
			case MessageType::ERROR: return "ERROR";
			default: return "UNKNOWN";
		}
	}

	MessageType FromString(const std::string& str);

}
