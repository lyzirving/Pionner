#ifndef __PIONNER_PLATFORM_WINDOWS_WINDOWS_THREAD_H__
#define __PIONNER_PLATFORM_WINDOWS_WINDOWS_THREAD_H__

#include "base/Thread.h"

namespace pio
{
	class WindowsThread : public Thread
	{
	public:
		WindowsThread(const std::string &name);
		virtual ~WindowsThread() = default;

		virtual void join() override;
		virtual void setName(const std::string &name) override;
	};
}

#endif