#include <asio.hpp>

#include "SensorNode.h"
#include "SmartFarm/Message.h"
#include "SmartFarm/Logger.h"

using asio::ip::tcp;

int main()
{
	SmartFarm::Logger::init();

	try
	{
		asio::io_context io;
		auto sensor = std::make_shared<SmartFarm::SensorNode>(
			io, "127.0.0.1", SmartFarm::Protocol::DEFAULT_PORT, 1);
		sensor->Start();
		io.run();
	}
	catch (const std::exception& e)
	{
		SmartFarm::Logger::error("Fatal error: {}", e.what());
	}
}