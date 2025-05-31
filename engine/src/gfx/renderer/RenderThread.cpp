#include "RenderThread.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "RenderThread"

namespace pio 
{
	RenderThread::RenderThread(const std::string &name)
	{
		m_Thread = Thread::Create(name);
	}

	RenderThread::~RenderThread()
	{
		m_Thread.reset(nullptr);
	}

	void RenderThread::Run(ThreadFunc&& func)
	{
		if(m_Thread->IsRunning())
			return;

		m_Thread->Bind(std::move(func));
		m_Thread->Start();
	}

	void RenderThread::Terminate()
	{
		Pump();
		m_Thread->Stop();
	}

	void RenderThread::Wait(State waitForState)
	{
		std::unique_lock<std::mutex> lk{ m_Mutex };
		while (m_State != waitForState)
		{
			m_Cv.wait(lk, [&] { return m_State == waitForState; });
		}
	}

	void RenderThread::WaitAndSet(State waitForState, State setToState)
	{
		{
			std::unique_lock<std::mutex> lk{ m_Mutex };
			while(m_State != waitForState)
			{
				m_Cv.wait(lk, [&] { return m_State == waitForState; });
			}
			m_State = setToState;
		}
		m_Cv.notify_all();
	}

	void RenderThread::Set(State setToState)
	{
		std::unique_lock<std::mutex> lk{ m_Mutex };
		m_State = setToState;
		m_Cv.notify_all();
	}

	void RenderThread::NextFrame()
	{
		m_ThreadFrame++;
	}

	void RenderThread::BlockUntilRenderComplete()
	{
		Wait(State::Idle);
	}

	void RenderThread::Kick()
	{
		Set(State::Kick);
	}

	void RenderThread::Pump()
	{
		NextFrame();
		Kick();
		BlockUntilRenderComplete();
	}

}
