#pragma once

#include "base/core/CoreDef.h"

namespace pio
{
	class SystemUtil
	{
		PIO_SINGLETON_DECLARE(SystemUtil)

	public:
		static uint64_t GetThreadID();
	};
}