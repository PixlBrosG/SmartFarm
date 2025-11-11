#include "Server.h"

#include "SmartFarm/Logger.h"

namespace SmartFarm {

	void Server::Start()
	{
		Logger::info("Server listening on port {}", m_Acceptor.local_endpoint().port());
		AcceptLoop();
	}

	void Server::AcceptLoop()
	{
		auto self = shared_from_this();
		m_Acceptor.async_accept(
			[this, self](std::error_code ec, asio::ip::tcp::socket socket)
			{
				if (!ec)
				{
					auto endpoint = socket.remote_endpoint();
					Logger::info("Accepted connection from {}:{}", endpoint.address().to_string(), endpoint.port());

					auto conn = std::make_shared<Connection>(std::move(socket));
					conn->Start([this, conn](const Message& msg) { OnMessage(conn, msg); });
				}
				else
				{
					Logger::error("Accept error: {}", ec.message());
				}

				// Keep accepting new clients
				AcceptLoop();
			});
	}

	void Server::OnMessage(const std::shared_ptr<Connection>& conn, const Message& msg)
	{
		using namespace Protocol;
		switch (msg.Type)
		{
		case MessageType::HELLO:
			HandleHello(conn, msg);
			break;
		case MessageType::SENSOR_UPDATE:
			HandleSensorUpdate(conn, msg);
			break;
		case MessageType::COMMAND:
			HandleCommand(conn, msg);
			break;
		default:
			Logger::warn("Unhandled message type {}", ToString(msg.Type));
		}
	}

	void Server::HandleHello(const std::shared_ptr<Connection>& conn, const Message& msg)
	{
		int nodeId = msg.Payload.value("node_id", -1);
		std::string role = msg.Payload.value("role", "unknown");
		Logger::info("Node {} registered as {}", nodeId, role);
		m_Nodes[nodeId] = conn;

		Message ack;
		ack.Type = Protocol::MessageType::ACK;
		ack.Payload = {{"message", "Node registered"}};
		conn->Send(ack);
	}

	void Server::HandleSensorUpdate(const std::shared_ptr<Connection>& conn, const Message& msg)
	{
		Logger::info("Sensor update: {}", msg.Payload.dump());
		// TODO: Implement (e.g. forward to control panels)
	}

	void Server::HandleCommand(const std::shared_ptr<Connection>& conn, const Message& msg)
	{
		Logger::info("Command: {}", msg.Payload.dump());
		// TODO: Implement (e.g. forward to target node)
	}

}
