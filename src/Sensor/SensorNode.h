#pragma once

#include <asio.hpp>

#include <memory>

#include "SmartFarm/Connection.h"

namespace SmartFarm {

	class SensorNode : public std::enable_shared_from_this<SensorNode>
	{
	public:
		SensorNode(asio::io_context& io, const std::string& host, uint16_t port, int id)
			: m_IO(io),
			  m_Socket(io),
			  m_Timer(io),
			  m_Host(host),
			  m_Port(port),
			  m_NodeId(id)
		{
		}

		void Start() { Connect(); }
	private:
		void Connect();
		void SendHello();
		void ScheduleUpdate();
		void SendSensorUpdate();
		void OnMessage(const Message& msg);
	private:
		asio::io_context& m_IO;
		asio::ip::tcp::socket m_Socket;
		std::shared_ptr<Connection> m_Conn;
		asio::steady_timer m_Timer;
		std::string m_Host;
		uint16_t m_Port;
		int m_NodeId;
	};

}
