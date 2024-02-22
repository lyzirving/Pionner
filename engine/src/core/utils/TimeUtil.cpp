#include <chrono>
#include <ctime>

#include "TimeUtil.h"

namespace pio
{
	uint64_t TimeUtil::CurrentTimeMicro()
	{
		auto now = std::chrono::steady_clock::now();
		auto timePt = std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch());
		return timePt.count();
	}

	uint64_t TimeUtil::CurrentTimeMs()
	{
		auto now = std::chrono::steady_clock::now();
		auto timePt = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());
		return timePt.count();
	}
}