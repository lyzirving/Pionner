#ifdef PIO_WINDOWS
	#include <Windows.h>
#endif

#include "SystemUtil.h"

namespace pio
{
	PIO_SINGLETON_IMPL(SystemUtil)

	void SystemUtil::Init(const InitParam& param)
	{
	}

	void SystemUtil::Shutdown()
	{
	}

	uint64_t SystemUtil::GetThreadID()
	{
		uint64_t id{ 0 };

		#ifdef PIO_WINDOWS
			id = GetCurrentThreadId();
		#endif // Windows

		return id;
	}
}