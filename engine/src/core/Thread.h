#ifndef __PIONNER_CORE_THREAD_H__
#define __PIONNER_CORE_THREAD_H__

#include <thread>

#include "Base.h"

namespace pio
{
	class Thread
	{
	public:
		Thread(const std::string &name) : m_name(name) {}
		virtual ~Thread() { if (m_thread.joinable()) { m_thread.detach(); } };

		virtual void join() = 0;
		virtual void setName(const std::string &name) = 0;

	public:
		inline bool isRunning() const { return m_isRunning; }
		inline void setRunning(bool r) { m_isRunning = r; }

		template<typename Fn, typename... Args>
		void dispatch(Fn &&func, Args&&... args)
		{
			m_thread = std::thread(func, std::forward<Args>(args)...);
			setName(m_name);
		}

	public:
		static Scope<Thread> create(const std::string &name);

	protected:
		std::string m_name{};
		std::thread m_thread;
		bool m_isRunning{ false };
	};
}

#endif