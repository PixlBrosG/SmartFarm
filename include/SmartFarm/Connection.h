#pragma once

#include <asio.hpp>

#include <functional>

#include "SmartFarm/Message.h"
#include "SmartFarm/Logger.h"

namespace SmartFarm {

	class Connection : public std::enable_shared_from_this<Connection>
	{
	public:
		using MessageHandler = std::function<void(const Message&)>;

		explicit Connection(asio::ip::tcp::socket socket)
			: m_Socket(std::move(socket)) {}

		void Start(MessageHandler handler);
		void Send(const Message& message);
	private:
		void ReadHeader();
		void ReadBody();
	private:
		asio::ip::tcp::socket m_Socket;
		MessageHandler m_Handler;
		uint32_t m_IncomingLength = 0;
		std::string m_IncomingData;
	};

}
