#include "SmartFarm/Connection.h"

#include "SmartFarm/Crypto.h"
#include "SmartFarm/Logger.h"

namespace SmartFarm {

	void Connection::Start(MessageHandler handler)
	{
		m_Handler = std::move(handler);
		ReadHeader();
	}

	void Connection::Send(const Message& message)
	{
		std::string payload = message.Serialize();
		payload = Crypto::XorEncryptDecrypt(payload);

		uint32_t length = htonl(static_cast<uint32_t>(payload.size()));
		std::vector<asio::const_buffer> buffers{
			asio::buffer(&length, sizeof(length)),
			asio::buffer(payload)
		};
		asio::write(m_Socket, buffers);
	}

	void Connection::SendError(const std::string& reason, int code)
	{
		Message err;
		err.Type = Protocol::MessageType::ERROR_MSG;
		err.Payload = {
			{"error", reason},
			{"code", code}
		};
		Send(err);
	}

	void Connection::ReadHeader()
	{
		auto self = shared_from_this();
		asio::async_read(m_Socket, asio::buffer(&m_IncomingLength, sizeof(m_IncomingLength)),
			[this, self](std::error_code ec, std::size_t)
			{
				if (!ec)
				{
					m_IncomingLength = ntohl(m_IncomingLength);
					m_IncomingData.resize(m_IncomingLength);
					ReadBody();
				}
				else
				{
					if (ec != asio::error::eof)
						Logger::error("Header read error: {}", ec.message());
					HandleDisconnect(ec);
				}
			});
	}

	void Connection::ReadBody()
	{
		auto self = shared_from_this();
		asio::async_read(m_Socket, asio::buffer(m_IncomingData),
			[this, self](std::error_code ec, std::size_t)
			{
				if (!ec)
				{
					m_IncomingData = Crypto::XorEncryptDecrypt(m_IncomingData);
					Message msg = Message::Deserialize(m_IncomingData);
					if (m_Handler) m_Handler(msg);
					ReadHeader();
				}
				else
				{
					if (ec != asio::error::eof)
						Logger::error("Body read error: {}", ec.message());
					HandleDisconnect(ec);
				}
			});
	}

	void Connection::HandleDisconnect(const std::error_code& ec)
	{
		if (ec == asio::error::eof || ec == asio::error::connection_reset)
			Logger::info("Client disconnected cleanly");
		else
			Logger::warn("Connection closed with error: {}", ec.message());

		if (m_OnDisconnect)
			m_OnDisconnect(shared_from_this());
	}

}
