#ifndef __CORE_TIME_UTIL_H__
#define __CORE_TIME_UTIL_H__

#include <cstdint>

namespace pio
{
	class TimeUtil
	{
	public:
		static uint64_t currentTimeMicro();
		static uint64_t currentTimeMs();

	private:
		TimeUtil() {};
		~TimeUtil() = default;
	};
}

#endif