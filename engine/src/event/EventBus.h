#ifndef __PIONNER_EVENT_EVENT_BUS_H__
#define __PIONNER_EVENT_EVENT_BUS_H__

#include <atomic>

#include "EBDef.h"
#include "core/Thread.h"

namespace pio
{
	class EventBusThread
	{
		friend class EventBus;
	public:
		enum class State
		{
			Idle = 0, Running, Waiting, Working, Quitting
		};
	public:
		EventBusThread(const std::string &name) : m_name(name) {}
		~EventBusThread();

		EventBusThread(const EventBusThread &rhs) 
			: m_state(rhs.m_state.load()),  m_name(rhs.m_name), m_taskStack(rhs.m_taskStack)
		{
		}

		void emplace(EventBusTaskFunc &&task);
		void start();
		void terminate();

		void consumeAll();
		void consumeOneTask();
		void waitIfEmpty();

	public:
		bool checkIdle() { { std::lock_guard<std::mutex> lk{ m_taskMutex }; return m_taskStack.empty(); } }
		uint32_t workLoad() { { std::lock_guard<std::mutex> lk{ m_taskMutex }; return m_taskStack.size(); } }

		bool isRunning() { return m_thread && m_thread->isRunning(); }

		void setState(State state) { m_state.store((uint8_t)state); }
		State getState() { return State(m_state.load()); }
		const std::string &getName() { return m_name; }
		
	private:
		static void Work(void *param);

	private:
		std::atomic_uint8_t m_state{ (uint8_t)State::Idle };

		std::string m_name{};
		Scope<Thread> m_thread;

		std::list<EventBusTaskFunc> m_taskStack{};
		std::mutex m_taskMutex{};
		std::condition_variable m_taskCond{};
	};

	/*
	* @brief  EventBus is used for time-costing task.
	*         User should submit() a task and call notity() at the end of task as a callback.
	*         EventBus will deal with the task in a work thread, and dispatch the notify in logic thread.
	*/
	class EventBus
	{
		PIO_SINGLETON_DECLARE(EventBus)
	public:
		void addRegister(PioEvent event, EventBusCb &&cb);
		void removeRegister(PioEvent event, EventBusCb &&cb);

		void dispatch();

		// notify() should be called in work thread when task is finished
		void notify(EventBusArg &&arg);

		void submit(EventBusTaskFunc &&task);

	private:
		void start();
		void terminate();

		void fireEvent(const EventBusArg &arg);
		
	private:
		static constexpr uint32_t THREAD_POOL_CNT = 3;
		EventBusThread m_pool[THREAD_POOL_CNT]{ EventBusThread("PIO-EB-0"),
			                                    EventBusThread("PIO-EB-1"),
			                                    EventBusThread("PIO-EB-2")};

		std::list<EventBusArg> m_eventStack{};
		std::mutex m_eventMutex{};

		std::unordered_map<uint8_t, std::vector<EventBusCb>> m_callbackList{};
		std::mutex m_cbMutex{};

		uint32_t m_curPoolInd{ 0 };
	};
}

#endif