#pragma once

#include <asio.hpp>

#include <unordered_map>
#include <memory>

#include "SmartFarm/Connection.h"
#include "SmartFarm/Message.h"

namespace SmartFarm {

	struct NodeInfo
	{
		Protocol::NodeRole Role;
		std::shared_ptr<Connection> Conn;
	};

	class Server : public std::enable_shared_from_this<Server>
	{
	public:
		explicit Server(asio::io_context& io, uint16_t port)
			: m_IO(io),
			  m_Acceptor(io, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port))
		{
		}

		void Start();
	private:
		void AcceptLoop();
		void HandleDisconnect(const std::shared_ptr<Connection>& conn);
		void OnMessage(const std::shared_ptr<Connection>& conn, const Message& msg);

		inline void HandleHello(const std::shared_ptr<Connection>& conn, const Message& msg);
		inline void HandleSensorUpdate(const std::shared_ptr<Connection>& conn, const Message& msg);
		inline void HandleCommand(const std::shared_ptr<Connection>& conn, const Message& msg);
		inline void HandleListNodes(const std::shared_ptr<Connection>& conn, const Message& msg);

		void BroadcastToControls(const Message& msg) const;
	private:
		asio::io_context& m_IO;
		asio::ip::tcp::acceptor m_Acceptor;

		std::unordered_map<int, NodeInfo> m_Sensors;
		std::vector<std::shared_ptr<Connection>> m_ControlPanels;

		int m_LastAssignedId = 0;
	};

}
