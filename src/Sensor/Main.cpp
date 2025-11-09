#include <asio.hpp>

#include "SmartFarm/Message.h"
#include "SmartFarm/Logger.h"

using asio::ip::tcp;

int main()
{
	SmartFarm::Logger::init();
	SmartFarm::Logger::info("Starting Sensor Node...");

	try
	{
		asio::io_context io;
		tcp::socket socket(io);
		socket.connect({asio::ip::make_address("127.0.0.1"), SmartFarm::Protocol::DEFAULT_PORT });

		// Create HELLO message
		SmartFarm::Message msg;
		msg.Type = SmartFarm::Protocol::MessageType::HELLO;
		msg.Payload = { { "node_id", 1 }, { "role", "sensor" } };

		auto data = msg.Serialize();
		asio::write(socket, asio::buffer(data));

		SmartFarm::Logger::info("HELLO sent.");

		// Wait for ACK
		std::array<char, 1024> buffer{};
		size_t len = socket.read_some(asio::buffer(buffer));
		std::string received(buffer.data(), len);

		SmartFarm::Logger::info("Received: {}", received);
	}
	catch (std::exception& e)
	{
		SmartFarm::Logger::error("Client error: {}", e.what());
	}
}