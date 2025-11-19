#pragma once

#include <asio.hpp>

#include "SmartFarm/Connection.h"

namespace SmartFarm {

	class ControlPanel : public std::enable_shared_from_this<ControlPanel>
	{
	public:
		ControlPanel(asio::io_context& io, const std::string& host, uint16_t port)
			: m_IO(io),
			  m_Socket(io),
			  m_Host(host),
			  m_Port(port)
		{
		}

		void Start() { Connect(); }
	private:
		void Connect();
		void SendHello();
		void CommandLoop();
		void OnMessage(const Message& msg);
	private:
		asio::io_context& m_IO;
		asio::ip::tcp::socket m_Socket;
		std::shared_ptr<Connection> m_Conn;
		std::string m_Host;
		uint16_t m_Port;
	};

}
