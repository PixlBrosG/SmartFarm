#include "SmartFarm/Protocol.h"

namespace SmartFarm::Protocol {

	MessageType FromString(const std::string& str)
	{
		if (str == "HELLO") return MessageType::HELLO;
		if (str == "ACK") return MessageType::ACK;
		if (str == "SENSOR_UPDATE") return MessageType::SENSOR_UPDATE;
		if (str == "ACTUATOR_STATUS") return MessageType::ACTUATOR_STATUS;
		if (str == "COMMAND") return MessageType::COMMAND;
		if (str == "ERROR") return MessageType::ERROR;
		return MessageType::ERROR;
	}

}
