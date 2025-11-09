#include <asio.hpp>

#include "SmartFarm/Logger.h"
#include "SmartFarm/Message.h"

using asio::ip::tcp;

int main()
{
	SmartFarm::Logger::init();
	SmartFarm::Logger::info("Starting SmartFarm Server...");

	try
	{
		asio::io_context io;
		tcp::acceptor acceptor(io, tcp::endpoint(tcp::v4(), SmartFarm::Protocol::DEFAULT_PORT));

		while (true)
		{
			tcp::socket socket(io);
			acceptor.accept(socket);
			SmartFarm::Logger::info("Client connected!");

			std::array<char, 1024> buffer{};
			size_t len = socket.read_some(asio::buffer(buffer));

			std::string received(buffer.data(), len);
			auto msg = SmartFarm::Message::Deserialize(received);

			SmartFarm::Logger::info("Received: {}", received);

			// Respond with ACK
			SmartFarm::Message reply;
			reply.Type = SmartFarm::Protocol::MessageType::ACK;
			reply.Payload = {{ "message", "Registered node" }};

			auto response = reply.Serialize();
			asio::write(socket, asio::buffer(response));

			SmartFarm::Logger::info("ACK sent.");
		}
	}
	catch (asio::system_error& e)
	{
		if (e.code() == asio::error::eof)
			SmartFarm::Logger::warn("Client disconnected normally (EOF)");
		else
			SmartFarm::Logger::error("Server error: {}", e.what());
	}
}
