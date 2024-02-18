#include "Thread.h"

#include "platform/windows/WindowsThread.h"

namespace pio
{
	Scope<Thread> Thread::create(const std::string &name)
	{
	#ifdef PIO_WINDOWS
		return CreateScope<WindowsThread>(name);
	#else
		return Scope<Thread>(nullptr);
	#endif
	}
}