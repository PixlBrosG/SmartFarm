#pragma once

#include <asio.hpp>

#include <unordered_map>
#include <memory>

#include "SmartFarm/Connection.h"
#include "SmartFarm/Message.h"

namespace SmartFarm {

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
		void OnMessage(const std::shared_ptr<Connection>& conn, const Message& msg);

		inline void HandleHello(const std::shared_ptr<Connection>& conn, const Message& msg);
		inline void HandleSensorUpdate(const std::shared_ptr<Connection>& conn, const Message& msg);
		inline void HandleCommand(const std::shared_ptr<Connection>& conn, const Message& msg);
	private:
		asio::io_context& m_IO;
		asio::ip::tcp::acceptor m_Acceptor;
		std::unordered_map<int, std::shared_ptr<Connection>> m_Nodes;
	};

}
