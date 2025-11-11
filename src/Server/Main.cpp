#include <asio.hpp>

#include "Server.h"
#include "SmartFarm/Logger.h"

using asio::ip::tcp;

int main()
{
	SmartFarm::Logger::init();

	try
	{
		asio::io_context io;
		auto server = std::make_shared<SmartFarm::Server>(io, SmartFarm::Protocol::DEFAULT_PORT);
		server->Start();
		io.run();
	}
	catch (const std::exception& e)
	{
		SmartFarm::Logger::error("Fatal error: {}", e.what());
	}
}
