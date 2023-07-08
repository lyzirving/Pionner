#ifdef WIN32
#include <Windows.h>
#endif // WIN32

#include "core/system/PlatformUtil.h"

#include "core/log/LogSystem.h"
#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "PlatformUtil"

namespace pio
{
	uint64_t PlatformUtil::getThreadId()
	{
		uint64_t id{ 0 };
		bool get{ false };
	#ifdef WIN32
		id = GetCurrentThreadId();
		get = true;
	#endif // WIN32
		if (!get)
		{
			LOG_ERR("fail to get thread id in current platform");
		}
		return id;
	}

	PlatformUtil::PlatformUtil()
	{
	}

	PlatformUtil::~PlatformUtil() = default;
}