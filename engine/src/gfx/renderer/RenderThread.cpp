#include "RenderThread.h"

#include "Renderer.h"

namespace pio 
{
	RenderThread::RenderThread(const std::string &name)
	{
		m_thread = Thread::create(name);
	}

	RenderThread::~RenderThread()
	{
		m_thread.reset(nullptr);
	}

	void RenderThread::run()
	{
		if (m_thread->isRunning())
			return;

		m_thread->setRunning(true);
		m_thread->dispatch(Renderer::RenderLoop, this);
	}

	void RenderThread::terminate()
	{
		m_thread->setRunning(false);
		pump();
		m_thread->join();
	}

	void RenderThread::wait(State waitForState)
	{
		std::unique_lock<std::mutex> lk{ m_mutex };
		while (m_state != waitForState)
		{
			m_cv.wait(lk, [&] { return m_state == waitForState; });
		}
	}

	void RenderThread::waitAndSet(State waitForState, State setToState)
	{
		std::unique_lock<std::mutex> lk{ m_mutex };
		while (m_state != waitForState)
		{
			m_cv.wait(lk, [&] { return m_state == waitForState; });
		}
		m_state = setToState;
		m_cv.notify_all();
	}

	void RenderThread::set(State setToState)
	{
		std::unique_lock<std::mutex> lk{ m_mutex };
		m_state = setToState;
		m_cv.notify_all();
	}

	void RenderThread::nextFrame()
	{
		m_threadFrame++;
		Renderer::SwapCmdQueues();
	}

	void RenderThread::blockUntilRenderComplete()
	{
		wait(State::Idle);
	}

	void RenderThread::kick()
	{
		set(State::Kick);
	}

	void RenderThread::pump()
	{
		nextFrame();
		kick();
		blockUntilRenderComplete();
	}

}
