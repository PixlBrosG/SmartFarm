#include "Server.h"

#include "SmartFarm/Logger.h"

namespace SmartFarm {

	void Server::Start()
	{
		Logger::info("Server listening on port {}", m_Acceptor.local_endpoint().port());
		AcceptLoop();
	}

	void Server::RemoveNode(int nodeId)
	{
		if (auto it = m_Nodes.find(nodeId); it != m_Nodes.end())
		{
			Logger::info("Node {} ({}) disconnected", nodeId, ToString(it->second.Role));
			m_Nodes.erase(it);
		}
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
		int requestedId = msg.Payload.value("node_id", -1);
		std::string roleStr = msg.Payload.value("role", "unknown");
		Protocol::NodeRole role = Protocol::NodeRoleFromString(roleStr);

		int assignedId = requestedId;
		if (requestedId == -1 || m_Nodes.contains(requestedId))
		{
			do
			{
				assignedId = ++m_LastAssignedId;
			}
			while (m_Nodes.contains(assignedId));
		}

		m_Nodes[assignedId] = { role, conn };

		Logger::info("Node {} registered as {}", assignedId, ToString(role));

		// ACK with assigned ID
		Message ack;
		ack.Type = Protocol::MessageType::ACK;
		ack.Payload = {
			{"message", "Node registered"},
			{"assigned_id", assignedId}
		};
		conn->Send(ack);
	}

	void Server::HandleSensorUpdate(const std::shared_ptr<Connection>& conn, const Message& msg)
	{
		Logger::info("Sensor update: {}", msg.Payload.dump());
		// TODO: Implement (e.g. forward to control panels)
	}

	void Server::HandleCommand(const std::shared_ptr<Connection>& conn, const Message& msg)
	{
		int targetNode = msg.Payload.value("target_node", -1);
		if (targetNode == -1)
		{
			Logger::warn("COMMAND missing target_node");
			// TODO: send error response
			return;
		}

		auto it = m_Nodes.find(targetNode);
		if (it == m_Nodes.end())
		{
			Logger::warn("COMMAND target node {} not found", targetNode);

			// Send error ACK to sender
			Message error;
			error.Type = Protocol::MessageType::ACK;
			error.Payload = {{"error", "Target not found"}};
			conn->Send(error);
			return;
		}

		auto& targetInfo = it->second;
		if (targetInfo.Role != Protocol::NodeRole::Sensor)
		{
			Logger::warn("COMMAND target {} is not a sensor", targetNode);
			// TODO: Send error response
			return;
		}

		Logger::info("Forwarding COMMAND from control panel to node {}", targetNode);
		targetInfo.Conn->Send(msg);

		// Confirm to control panel
		Message ack;
		ack.Type = Protocol::MessageType::ACK;
		ack.Payload = {{"message", "Command forwarded"}, {"target_node", targetNode}};
		conn->Send(ack);
	}

}
