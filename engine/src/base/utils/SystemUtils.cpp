#ifdef PIO_WINDOWS
	#include <Windows.h>
#endif

#include "SystemUtils.h"

namespace pio
{
	PIO_SINGLETON_IMPL(SystemUtils)

	SystemUtils::SystemUtils()
	{
	}

	SystemUtils::~SystemUtils() = default;

	void SystemUtils::Init()
	{
	}

	void SystemUtils::Shutdown()
	{
	}

	uint64_t SystemUtils::GetThreadId()
	{
		uint64_t id{ 0 };

		#ifdef PIO_WINDOWS
			id = GetCurrentThreadId();
		#endif // Windows

		return id;
	}
}