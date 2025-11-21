#include "SmartFarm/Protocol.h"

namespace SmartFarm::Protocol {

	MessageType MessageTypeFromString(const std::string& str)
	{
		if (str == "HELLO") return MessageType::HELLO;
		if (str == "ACK") return MessageType::ACK;
		if (str == "SENSOR_UPDATE") return MessageType::SENSOR_UPDATE;
		if (str == "ACTUATOR_STATUS") return MessageType::ACTUATOR_STATUS;
		if (str == "COMMAND") return MessageType::COMMAND;
		if (str == "ERROR") return MessageType::ERROR_MSG;
		if (str == "LIST_NODES") return MessageType::LIST_NODES;
		if (str == "NODE_LIST") return MessageType::NODE_LIST;
		return MessageType::ERROR_MSG;
	}

	NodeRole NodeRoleFromString(const std::string& str)
	{
		if (str == "sensor") return NodeRole::Sensor;
		if (str == "control") return NodeRole::Control;
		return NodeRole::Unknown;
	}

}
