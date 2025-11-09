#pragma once

#include <string>
#include <chrono>

namespace SmartFarm::Util {

	inline long Timestamp()
	{
		return std::chrono::duration_cast<std::chrono::seconds>(
			std:chrono::steady_clock::now().time_since_epoch()
		).count();
	}

}
