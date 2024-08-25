#ifndef __PIONNER_BASE_UTILS_SYSTEM_UTILS_H__
#define __PIONNER_BASE_UTILS_SYSTEM_UTILS_H__

#include "Entry.h"

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