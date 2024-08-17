#ifndef __PIONNER_PLATFORM_SYSTEM_UTILS_H__
#define __PIONNER_PLATFORM_SYSTEM_UTILS_H__

#include "base/Base.h"

namespace pio
{
	class SystemUtils
	{
		PIO_SINGLETON_DECLARE(SystemUtils)

	public:
		static uint64_t GetThreadId();
	};
}

#endif