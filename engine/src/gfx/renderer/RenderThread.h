#pragma once

#include "base/core/Thread.h"

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
		
		void Run(ThreadFunc&& func);
		void Terminate();

		void Wait(State waitForState);
		void WaitAndSet(State waitForState, State setToState);
		void Set(State setToState);

		void NextFrame();
		void BlockUntilRenderComplete();
		void Kick();

		void Pump();

	public:
		bool IsRunning() const { return m_Thread && m_Thread->IsRunning(); }

	private:
		State m_State{ State::Idle };

		Scope<Thread> m_Thread;
		std::atomic_uint32_t m_ThreadFrame{ 0 };

		std::mutex m_Mutex{};
		std::condition_variable m_Cv{};
	};

}
