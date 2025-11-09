#pragma once
#include <spdlog/spdlog.h>

namespace SmartFarm::Logger {

	inline void init()
	{
		spdlog::set_pattern("[%H:%M:%S] [%^%l%$] %v");
		spdlog::set_level(spdlog::level::debug);
	}

	using spdlog::info;
	using spdlog::warn;
	using spdlog::error;
	using spdlog::debug;

}
