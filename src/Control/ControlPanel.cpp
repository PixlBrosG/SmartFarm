#include "ControlPanel.h"

#include <iostream>

namespace SmartFarm {

	void ControlPanel::Connect()
	{
		asio::ip::tcp::resolver resolver(m_IO);
		auto endpoints = resolver.resolve(m_Host, std::to_string(m_Port));

		auto self = shared_from_this();
		asio::async_connect(m_Socket, endpoints,
			[this, self](std::error_code ec, const asio::ip::tcp::endpoint&)
			{
				if (!ec)
				{
					Logger::info("Connected to server.");
					m_Conn = std::make_shared<Connection>(std::move(m_Socket));
					m_Conn->Start([self](const Message& msg) { self->OnMessage(msg); });
					SendHello();
					CommandLoop();
				}
				else
				{
					Logger::error("Connect failed: {}", ec.message());
				}
			});
	}

	void ControlPanel::SendHello()
	{
		Message msg;
		msg.Type = Protocol::MessageType::HELLO;
		msg.Payload = {{ "node_id", 999 }, { "role", ToString(Protocol::NodeRole::Control) }};
		m_Conn->Send(msg);
		Logger::info("HELLO sent (control panel).");
	}

	void ControlPanel::CommandLoop()
	{
		std::thread([self = shared_from_this()] {
			while (true) {
				std::string line;
				std::getline(std::cin, line);

				if (line == "quit" || line == "exit") break;

				// Example input: send 1 fan ON
				std::istringstream iss(line);
				std::string cmd;
				iss >> cmd;
				if (cmd == "send") {
					int targetNode;
					std::string targetActuator, action;
					iss >> targetNode >> targetActuator >> action;

					Message msg;
					msg.Type = Protocol::MessageType::COMMAND;
					msg.Payload = {
						{"target_node", targetNode},
						{"target_actuator", targetActuator},
						{"action", action}
					};

					self->m_Conn->Send(msg);
					Logger::info("Sent COMMAND: {}", msg.Payload.dump());
				} else {
					Logger::warn("Unknown command '{}'", cmd);
				}
			}
		}).detach();
	}

	void ControlPanel::OnMessage(const Message& msg)
	{
		using namespace Protocol;
		switch (msg.Type)
		{
		case MessageType::SENSOR_UPDATE:
		{
			int node = msg.Payload.value("node_id", -1);
			double temp = msg.Payload.value("temperature", 0.0);
			double humidity = msg.Payload.value("humidity", 0.0);
			Logger::info("[Node {}] Temp: {:.2f}°C, Humidity: {:.1f}%", node, temp, humidity);
			break;
		}
		case MessageType::ACTUATOR_STATUS:
			Logger::info("Actuator status: {}", msg.Payload.dump());
			break;
		case MessageType::ACK:
			// NOTE: Control Panel doesn't need to know about its ID, so we can ignore assigned ID
			Logger::debug("Received ACK: {}", msg.Payload.dump());
			break;
		case MessageType::ERROR:
			Logger::error("Received error: {}", msg.Payload.dump());
		default:
			Logger::debug("Unhandled message type: {}", ToString(msg.Type));
		}
	}

}
