#include "EventHub.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "EventHub"

namespace pio
{
	PIO_SINGLETON_IMPL(EventHub)

	EventHub::EventHub() {}

	EventHub::~EventHub() = default;

	void EventHub::Init()
	{
		EventHub::Get();
	}

	void EventHub::Shutdown()
	{
		std::lock_guard<std::mutex> lk{ s_mutex };
		delete s_instance;
		s_instance = nullptr;
	}

	void EventHub::add(Ref<Event> &event)
	{
		std::lock_guard<std::mutex> lk{ m_stackMutex };
		m_stack.push_back(event);
	}

	void EventHub::registerCallback(EventHubCb &&cb)
	{
		std::lock_guard<std::mutex> lk{ m_cbMutex };
		for(auto &item : m_cbList)
		{
			if(cb == item)
			{
				return;
			}
		}
		m_cbList.push_back(std::move(cb));
	}

	void EventHub::removeCallback(EventHubCb &&cb)
	{
		std::lock_guard<std::mutex> lk{ m_cbMutex };
		auto itor = m_cbList.begin();
		while(itor != m_cbList.end())
		{
			auto &item = *itor;
			if(item == cb)
			{
				itor = m_cbList.erase(itor);
				return;
			}
		}
	}

	void EventHub::dispatch()
	{
		std::lock_guard<std::mutex> lk{ m_stackMutex };
		while(!m_stack.empty())
		{
			Ref<Event> e = m_stack.front();
			m_stack.pop_front();
			dispatchToCallbackList(e);
		}
	}

	void EventHub::dispatchToCallbackList(Ref<Event> &event)
	{
		std::lock_guard<std::mutex> lk{ m_cbMutex };
		for(auto &cb : m_cbList)
		{			
			(cb.Target->*cb.Cb)(event);			
		}
	}
}