#include "SensorNode.h"

#include "SmartFarm/SensorData.h"
#include "SmartFarm/Util.h"

namespace SmartFarm {

	void SensorNode::Connect()
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
					ScheduleUpdate();
				}
				else
				{
					Logger::error("Connection failed: {}", ec.message());
					// TODO: Retry later
				}
			});
	}

	void SensorNode::SendHello()
	{
		Message msg;
		msg.Type = Protocol::MessageType::HELLO;
		msg.Payload = {{ "node_id", m_NodeId }, { "role", ToString(Protocol::NodeRole::Sensor) }};
		m_Conn->Send(msg);
		Logger::info("HELLO sent (node {}).", m_NodeId);
	}

	void SensorNode::ScheduleUpdate()
	{
		m_Timer.expires_after(std::chrono::seconds(5));
		m_Timer.async_wait([self = shared_from_this()](std::error_code ec)
		{
			if (!ec)
			{
				self->SendSensorUpdate();
				self->ScheduleUpdate();
			}
		});
	}

	void SensorNode::SendSensorUpdate()
	{
		SensorData data;
		data.Randomize();

		Message msg;
		msg.Type = Protocol::MessageType::SENSOR_UPDATE;
		msg.Payload = {
			{ "node_id", m_NodeId },
			{ "timestamp", Util::Timestamp() },
			{ "temperature", data.Temperature },
			{ "humidity", data.Humidity }
		};
		m_Conn->Send(msg);
		Logger::info("Sent SENSOR_UPDATE from node {}.", m_NodeId);
	}

	void SensorNode::OnMessage(const Message& msg)
	{
		using namespace Protocol;
		if (msg.Type == MessageType::COMMAND)
		{
			std::string actuator = msg.Payload.value("target_actuator", "unknown");
			std::string action = msg.Payload.value("action", "unknown");
			Logger::info("Received COMMAND: {} -> {}", actuator, action);

			// TODO: Update status

			Message status;
			status.Type = Protocol::MessageType::ACTUATOR_STATUS;
			status.Payload = {
				{"node_id", m_NodeId},
				{"actuator", actuator},
				{"status", action}
			};
			m_Conn->Send(status);
		}
		else
		{
			Logger::debug("Unhandled message type: {}", ToString(msg.Type));
		}
	}

}
