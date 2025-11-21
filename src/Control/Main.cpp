#include <asio.hpp>

#include "ControlPanel.h"
#include "SmartFarm/Logger.h"

int main() {
	SmartFarm::Logger::init();

	try {
		asio::io_context io;
		auto control = std::make_shared<SmartFarm::ControlPanel>(
			io, "127.0.0.1", SmartFarm::Protocol::DEFAULT_PORT);
		control->Start();
		io.run();
	}
	catch (const std::exception& e)
	{
		SmartFarm::Logger::error("Fatal error: {}", e.what());
	}
}
