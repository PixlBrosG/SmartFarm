#include "SmartFarm/SensorData.h"

#include <random>

namespace SmartFarm {

	void SensorData::Randomize()
	{
		static std::mt19937 rng(std::random_device{}());
		std::uniform_real_distribution<double> tdist(18.0, 26.0);
		std::uniform_real_distribution<double> hdist(30.0, 70.0);

		Temperature = tdist(rng);
		Humidity = hdist(rng);
	}

}
