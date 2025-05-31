#pragma once

#include "CoreDef.h"

namespace pio
{
	using TStatus = uint8_t;
	using ThreadFunc = std::function<void()>;

	namespace ThreadStatus
	{
		enum : TStatus
		{
			Idle,
			Running,
			RequestQuit
		};
	}

	class Thread
	{
	public:
		Thread();
		Thread(const std::string& name);
		virtual ~Thread();

		void Bind(ThreadFunc&& fun) { m_Func = std::move(fun); }

		void Start();
		void Stop();
		void RequestQuit() { m_Status = ThreadStatus::RequestQuit; }
		void Join();
		bool IsRunning() const { return m_Status == ThreadStatus::Running; }

	public:
		static Scope<Thread> Create(const std::string& name);

	protected:
		void Run();
		//Set thread name acorrding to platform
		virtual void SetThreadName(const std::string& name) = 0;

	private:
		static uint32_t k_ThreadNum;

	protected:
		ThreadFunc m_Func;
		std::thread m_Thread;
		const std::string m_Name;
		std::atomic<TStatus> m_Status{ ThreadStatus::Idle };
	};
}