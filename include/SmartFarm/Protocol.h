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
		ERROR,
		LIST_NODES,
		NODE_LIST
	};

	enum class NodeRole : uint8_t
	{
		Unknown,
		Sensor,
		Control
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
			case MessageType::LIST_NODES: return "LIST_NODES";
			case MessageType::NODE_LIST: return "NODE_LIST";
			default: return "UNKNOWN";
		}
	}

	MessageType MessageTypeFromString(const std::string& str);

	inline std::string ToString(NodeRole role)
	{
		switch (role)
		{
			case NodeRole::Sensor: return "sensor";
			case NodeRole::Control: return "control";
			default: return "unknown";
		}
	}

	NodeRole NodeRoleFromString(const std::string& str);

}
