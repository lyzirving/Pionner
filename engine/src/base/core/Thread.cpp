#include "Thread.h"

#include "base/utils/LogSystem.h"
#include "platform/windows/WindowsThread.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Thread"

namespace pio
{
	uint32_t Thread::k_ThreadNum{ 0 };

	Scope<Thread> Thread::Create(const std::string& name)
	{
	#ifdef PIO_WINDOWS
		return CreateScope<WindowsThread>(name);
	#else
		return Scope<Thread>(nullptr);
	#endif
	}

	Thread::Thread() : m_Name(std::string("UnknownThread-")
							  .append(std::to_string(k_ThreadNum++)))
	{
	}

	Thread::Thread(const std::string& name) : m_Name(std::string(name))
	{
		++k_ThreadNum;
	}

	Thread::~Thread()
	{
		Stop();
		--k_ThreadNum;
	}

	void Thread::Start()
	{
		if(!m_Func)
		{
			LOGE("thread[%s] hasn't been bound!", m_Name.c_str());
			return;
		}

		m_Status = ThreadStatus::Running;
		m_Thread = std::thread(&Thread::Run, this);
		SetThreadName(m_Name); 
	}

	void Thread::Stop()
	{
		if(m_Status != ThreadStatus::Running)
			return;

		m_Status = ThreadStatus::RequestQuit;
		LOGD("thread[%s] requests quit", m_Name.c_str());
		Join();
		LOGD("thread[%s] stopped", m_Name.c_str());
	}

	void Thread::Join()
	{
		if(m_Thread.joinable())
		{
			m_Thread.join();
		}
	}

	void Thread::Run()
	{
		LOGD("thread[%s] begins to run", m_Name.c_str());
		while(m_Status == ThreadStatus::Running)
		{
			m_Func();
		}
		LOGD("thread[%s] quits", m_Name.c_str());
		m_Status = ThreadStatus::Idle;
	}
}
