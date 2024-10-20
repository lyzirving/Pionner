#ifndef __PIONNER_BASE_UTILS_SYSTEM_UTIL_H__
#define __PIONNER_BASE_UTILS_SYSTEM_UTIL_H__

#include "base/defs/FunDef.h"

namespace pio
{
	class SystemUtil
	{
		PIO_SINGLETON_DECLARE(SystemUtil)

	public:
		static uint64_t GetThreadId();
	};
}

#endif