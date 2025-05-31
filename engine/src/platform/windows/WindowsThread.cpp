#include "WindowsThread.h"

#include <Windows.h>

namespace pio
{
	WindowsThread::WindowsThread(const std::string &name) : Thread(name)
	{
	}

	void WindowsThread::SetThreadName(const std::string &name)
	{
		HANDLE threadHandle = m_Thread.native_handle();

		std::wstring wName(name.begin(), name.end());
		SetThreadDescription(threadHandle, wName.c_str());
		SetThreadAffinityMask(threadHandle, 8);
	}
}