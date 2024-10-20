#ifdef PIO_WINDOWS
	#include <Windows.h>
#endif

#include "SystemUtil.h"

namespace pio
{
	PIO_SINGLETON_IMPL(SystemUtil)

	SystemUtil::SystemUtil()
	{
	}

	SystemUtil::~SystemUtil() = default;

	void SystemUtil::Init()
	{
	}

	void SystemUtil::Shutdown()
	{
	}

	uint64_t SystemUtil::GetThreadId()
	{
		uint64_t id{ 0 };

		#ifdef PIO_WINDOWS
			id = GetCurrentThreadId();
		#endif // Windows

		return id;
	}
}