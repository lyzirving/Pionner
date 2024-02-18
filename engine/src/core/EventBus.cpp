#include "EventBus.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "EventBus"

namespace pio
{
	EventBusThread::~EventBusThread()
	{
		terminate();
	}

	void EventBusThread::emplace(EventBusTaskFunc &&task)
	{
		std::lock_guard<std::mutex> lk{ m_taskMutex };
		m_taskStack.push_back(std::forward<EventBusTaskFunc>(task));
		m_taskCond.notify_all();
	}

	void EventBusThread::start()
	{
		if (m_thread)
		{
			m_thread->setRunning(false);
			m_taskCond.notify_all();
			m_thread->join();
		}
		m_thread = Thread::create(m_name);
		m_thread->setRunning(true);
		m_thread->dispatch(EventBusThread::Work, this);
	}

	void EventBusThread::terminate()
	{
		m_thread->setRunning(false);
		m_taskCond.notify_all();
		m_thread->join();
	}

	void EventBusThread::consumeAll()
	{
		setState(State::Working);
		bool empty{ false };
		while (!empty)
		{
			consumeOneTask();
			empty = checkIdle();
		}
	}

	void EventBusThread::consumeOneTask()
	{
		EventBusTaskFunc func{};
		{
			std::lock_guard<std::mutex> lk{ m_taskMutex };
			if (!m_taskStack.empty())
			{
				func = m_taskStack.front();
				m_taskStack.pop_front();
			}
		}
		if (func && m_thread->isRunning())
			func();
	}

	void EventBusThread::waitIfEmpty()
	{
		if (checkIdle())
		{
			std::unique_lock<std::mutex> lk{ m_taskMutex };
			setState(State::Waiting);
			m_taskCond.wait(lk, [&] { return !m_thread->isRunning() || !m_taskStack.empty(); });
		}
	}

	void EventBusThread::Work(void *param)
	{
		auto *self = static_cast<EventBusThread *>(param);
		LOGD("enter event bus work thread[%s]\n", self->getName().c_str());
		self->setState(State::Running);
		while (self->isRunning())
		{
			self->waitIfEmpty();
			self->consumeAll();
		}
		self->setState(State::Quitting);
		LOGD("exit event bus work thread[%s]\n", self->getName().c_str());
	}

	PIO_SINGLETON_IMPL(EventBus)

	EventBus::EventBus()
	{
	}

	EventBus::~EventBus()
	{
	}

	void EventBus::Init()
	{
		EventBus::Get()->start();
	}

	void EventBus::Shutdown()
	{
		EventBus::Get()->terminate();

		std::lock_guard<std::mutex> lk{ s_mutex };
		delete s_instance;
		s_instance = nullptr;
	}

	void EventBus::submit(EventBusTaskFunc &&task)
	{
		m_pool[m_curPoolInd].emplace(std::forward<EventBusTaskFunc>(task));
		m_curPoolInd = (m_curPoolInd + 1) % THREAD_POOL_CNT;
	}

	void EventBus::addRegister(PioEvent event, EventBusCb &&cb)
	{
		std::lock_guard<std::mutex> lk{ m_cbMutex };
		auto it = m_callbackList.find((uint8_t)event);
		if (it != m_callbackList.end())
		{
			bool found = false;
			auto &list = it->second;
			for (auto &item : list)
			{
				if (item == cb)
				{
					found = true;
					break;
				}
			}
			if (!found)
				list.push_back(std::forward<EventBusCb>(cb));
		}
		else
		{
			std::vector<EventBusCb> list{};
			list.push_back(std::forward<EventBusCb>(cb));
			m_callbackList[(uint8_t)event] = list;
		}
	}

	void EventBus::removeRegister(PioEvent event, EventBusCb &&cb)
	{
		std::lock_guard<std::mutex> lk{ m_cbMutex };
		auto it = m_callbackList.find((uint8_t)event);
		if (it != m_callbackList.end())
		{
			auto &list = it->second;
			auto listIt = list.begin();
			while (listIt != list.end())
			{
				if ((*listIt) == cb)
				{
					list.erase(listIt);
					break;
				}
				else
					listIt++;
			}
		}
	}

	void EventBus::dispatch()
	{
		std::lock_guard<std::mutex> lk{ m_eventMutex };
		while (!m_eventStack.empty())
		{
			auto &arg = m_eventStack.front();
			fireEvent(arg);
			m_eventStack.pop_front();
		}
	}

	void EventBus::notify(EventBusArg &&arg)
	{
		std::lock_guard<std::mutex> lk{ m_eventMutex };
		m_eventStack.push_back(std::forward<EventBusArg>(arg));
	}

	void EventBus::start()
	{
		for (auto &t : m_pool)
			t.start();
	}

	void EventBus::terminate()
	{
		for (auto &t : m_pool)
			t.terminate();
	}

	void EventBus::fireEvent(const EventBusArg &arg)
	{
		std::lock_guard<std::mutex> lk{ m_cbMutex };
		auto it = m_callbackList.find((uint8_t)arg.Event);
		if (it != m_callbackList.end())
		{
			auto &list = it->second;
			for (auto &item : list)
			{
				(item.Target->*item.Cb)(arg);
			}
		}
	}
}