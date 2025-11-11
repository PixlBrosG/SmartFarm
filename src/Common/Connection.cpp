#include "SmartFarm/Connection.h"

namespace SmartFarm {

	void Connection::Start(MessageHandler handler)
	{
		m_Handler = std::move(handler);
		ReadHeader();
	}

	void Connection::Send(const Message& message)
	{
		std::string payload = message.Serialize();
		uint32_t length = htonl(static_cast<uint32_t>(payload.size()));
		std::vector<asio::const_buffer> buffers{
			asio::buffer(&length, sizeof(length)),
			asio::buffer(payload)
		};
		asio::write(m_Socket, buffers);
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
				else if (ec != asio::error::eof)
				{
					Logger::error("Header read error: {}", ec.message());
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
					Message msg = Message::Deserialize(m_IncomingData);
					if (m_Handler) m_Handler(msg);
					ReadHeader();
				}
				else if (ec != asio::error::eof)
				{
					Logger::error("Body read error: {}", ec.message());
				}
			});
	}

}
