#ifndef __PLATFORM_UTIL_H__
#define __PLATFORM_UTIL_H__

#include <cstdint>

namespace pio
{
	class PlatformUtil
	{
	public:
		static uint64_t getThreadId();

	private:
		PlatformUtil();
		~PlatformUtil();
	};
}

#endif