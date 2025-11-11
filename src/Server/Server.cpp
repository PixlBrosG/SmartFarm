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
					conn->SetDisconnectHandler([this](auto c) { HandleDisconnect(c); });
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

	void Server::HandleDisconnect(const std::shared_ptr<Connection>& conn) {
		// erase from m_Sensors if found
		for (auto it = m_Sensors.begin(); it != m_Sensors.end(); ++it) {
			if (it->second.Conn == conn) {
				Logger::info("Sensor {} disconnected", it->first);
				m_Sensors.erase(it);
				return;
			}
		}
		// otherwise remove from control panels
		m_ControlPanels.erase(
			std::ranges::remove(m_ControlPanels, conn).begin(),
			m_ControlPanels.end()
		);
		Logger::info("Control panel disconnected");
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
		auto role = Protocol::NodeRoleFromString(roleStr);

		if (role == Protocol::NodeRole::Unknown) {
			Logger::warn("HELLO with unknown role '{}'", roleStr);
			Message err;
			err.Type = Protocol::MessageType::ACK;
			err.Payload = {{"error", "Invalid role"}};
			conn->Send(err);
			return;
		}

		int assignedId = requestedId;

		if (role == Protocol::NodeRole::Sensor) {
			// Assign an ID only for sensors
			if (requestedId == -1 || m_Sensors.contains(requestedId)) {
				do {
					assignedId = ++m_LastAssignedId;
				} while (m_Sensors.contains(assignedId));
			}

			m_Sensors[assignedId] = {role, conn};
			Logger::info("Sensor node {} registered", assignedId);
		}
		else if (role == Protocol::NodeRole::Control) {
			// No unique ID needed for control panels
			m_ControlPanels.push_back(conn);
			Logger::info("Control panel connected");
		}

		// ACK with optional assigned ID
		Message ack;
		ack.Type = Protocol::MessageType::ACK;
		ack.Payload = {{"message", "Node registered"}};
		if (role == Protocol::NodeRole::Sensor)
			ack.Payload["assigned_id"] = assignedId;

		conn->Send(ack);
	}

	void Server::HandleSensorUpdate(const std::shared_ptr<Connection>& conn, const Message& msg)
	{
		int nodeId = msg.Payload.value("node_id", -1);
		if (nodeId == -1)
		{
			Logger::warn("Received SENSOR_UPDATE with missing node_id");
			return;
		}

		// Verify this node is actually registered as a sensor
		auto it = m_Sensors.find(nodeId);
		if (it == m_Sensors.end())
		{
			Logger::warn("Received SENSOR_UPDATE from unregistered node {}", nodeId);
			return;
		}

		Logger::info("Forwarding data from node {} to {} control panel(s)", nodeId, m_ControlPanels.size());

		BroadcastToControls(msg);
	}

	void Server::HandleCommand(const std::shared_ptr<Connection>& conn, const Message& msg)
	{
		int targetNode = msg.Payload.value("target_node", -1);
		if (targetNode == -1)
		{
			Logger::warn("COMMAND missing target_node");
			conn->SendError("Missing target_node");
			return;
		}

		auto it = m_Sensors.find(targetNode);
		if (it == m_Sensors.end())
		{
			Logger::warn("COMMAND target node {} not found", targetNode);
			conn->SendError("Target node not found");
			return;
		}

		auto& targetInfo = it->second;
		if (targetInfo.Role != Protocol::NodeRole::Sensor)
		{
			Logger::warn("COMMAND target {} is not a sensor", targetNode);
			conn->SendError("Target is not a sensor");
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

	void Server::BroadcastToControls(const Message& msg) const
	{
		for (auto& controlPanel : m_ControlPanels)
		{
			try
			{
				controlPanel->Send(msg);
			}
			catch (const std::exception& e)
			{
				Logger::error("Failed to broadcast to control panel: {}", e.what());
			}
		}
	}


}
