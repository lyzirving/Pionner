#pragma once

#include "base/core/Thread.h"

namespace pio
{
	class WindowsThread : public Thread
	{
	public:
		WindowsThread(const std::string &name);
		virtual ~WindowsThread() = default;

	protected:
		virtual void SetThreadName(const std::string& name) override;
	};
}