#ifndef __PIONNER_GFX_RENDERER_RENDER_THREAD_H__
#define __PIONNER_GFX_RENDERER_RENDER_THREAD_H__

#include "base/Thread.h"

namespace pio
{
	class RenderThread
	{
	public:
		enum class State : uint8_t
		{
			Idle = 0,
			Busy,
			Kick
		};

	public:
		RenderThread(const std::string &name = "RenderThread");
		~RenderThread();
		
		void run(std::function<void()> func);
		void terminate();

		void wait(State waitForState);
		void waitAndSet(State waitForState, State setToState);
		void set(State setToState);

		void nextFrame();
		void blockUntilRenderComplete();
		void kick();

		void pump();

	public:
		bool isRunning() const { return m_thread && m_thread->isRunning(); }

	private:
		State m_state{ State::Idle };

		Scope<Thread> m_thread;
		std::atomic_uint32_t m_threadFrame{ 0 };

		std::mutex m_mutex{};
		std::condition_variable m_cv{};
	};

}

#endif
