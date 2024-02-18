#include "WindowsThread.h"

#include <Windows.h>

namespace pio
{
	WindowsThread::WindowsThread(const std::string &name) : Thread(name)
	{
	}

	void WindowsThread::join()
	{
		if (m_thread.joinable())
			m_thread.join();
	}

	void WindowsThread::setName(const std::string &name)
	{
		HANDLE threadHandle = m_thread.native_handle();

		std::wstring wName(name.begin(), name.end());
		SetThreadDescription(threadHandle, wName.c_str());
		SetThreadAffinityMask(threadHandle, 8);
	}
}