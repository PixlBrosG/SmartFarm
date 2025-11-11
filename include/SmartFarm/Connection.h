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
		using DisconnectHandler = std::function<void(std::shared_ptr<Connection>)>;

		explicit Connection(asio::ip::tcp::socket socket)
			: m_Socket(std::move(socket)) {}

		void Start(MessageHandler handler);
		void Send(const Message& message);

		void SetDisconnectHandler(DisconnectHandler handler) { m_OnDisconnect = std::move(handler); }
	private:
		void ReadHeader();
		void ReadBody();
		void HandleDisconnect(const std::error_code& ec);

	private:
		asio::ip::tcp::socket m_Socket;
		MessageHandler m_Handler;
		DisconnectHandler m_OnDisconnect;

		uint32_t m_IncomingLength = 0;
		std::string m_IncomingData;
	};

}
