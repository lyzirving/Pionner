#include "EventMgr.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "EventMgr"

namespace pio
{
	PIO_SINGLETON_IMPL(EventMgr)

	void EventMgr::Init(const InitParam& param)
	{
	}

	void EventMgr::Shutdown()
	{
	}

	void EventMgr::PushBackWin(Ref<Event>& event)
	{
		std::lock_guard<std::mutex> lk{ m_StackMutex };
		m_Stack.push_back(event);
	}

	void EventMgr::ConnectWin(EventCallback&& cb)
	{
		std::lock_guard<std::mutex> lk{ m_WinMutex };
		for(auto& item : m_WinHandlers)
		{
			if(cb == item)
			{
				LOGW("callback already exist, t[%p], cb[%p]",
					 cb.Target, cb.Func);
				return;
			}
		}
		m_WinHandlers.push_back(std::move(cb));
	}

	void EventMgr::RemoveWin(EventCallback&& cb)
	{
		std::lock_guard<std::mutex> lk{ m_WinMutex };
		auto itor = m_WinHandlers.begin();
		while(itor != m_WinHandlers.end())
		{
			auto& item = *itor;
			if(item == cb)
			{
				itor = m_WinHandlers.erase(itor);
				return;
			}
		}
	}

	void EventMgr::BroadcastEvent(const Ref<Event>& event)
	{
		std::lock_guard<std::mutex> lk{ m_SlotEvtMutex };
		m_SlotEvents.push_back(event);
	}

	void EventMgr::Dispatch()
	{
		DispatchWinHandlers();
		DispatchEvent();
	}

	void EventMgr::AddSlotCallback(uint32_t id, EventCallback&& cb)
	{
		std::lock_guard<std::mutex> lk{ m_MapMutex };
		auto it = m_SlotMap.find(id);
		if(it == m_SlotMap.end())
		{
			std::vector<EventCallback> list;
			list.push_back(std::move(cb));
			m_SlotMap.insert({ id, list });
		}
		else
		{
			auto& list = it->second;
			auto ret = std::find(list.begin(), list.end(), cb);
			if(ret == list.end())
			{
				list.push_back(std::move(cb));
			}
			else
			{
				LOGW("event[%u]'s callback already exist, t[%p], func[%p]",
					 id, cb.Target, cb.Func);
			}
		}
	}

	void EventMgr::RemoveSlotCallback(uint32_t id, EventCallback&& cb)
	{
		std::lock_guard<std::mutex> lk{ m_MapMutex };
		auto it = m_SlotMap.find(id);
		if(it != m_SlotMap.end())
		{
			auto& list = it->second;
			auto ret = std::find(list.begin(), list.end(), cb);
			if(ret != list.end())
			{
				list.erase(ret);
			}
			else
			{
				LOGE("fail to find callback[%p / %p] inst for event[%u]",
					 cb.Target, cb.Func, id);
			}
		}
		else
		{
			LOGE("no callback for event[%u] exist", id);
		}
	}

	void EventMgr::DispatchWinHandlers()
	{
		std::lock_guard<std::mutex> lk{ m_StackMutex };
		while(!m_Stack.empty())
		{
			auto e = m_Stack.front();
			m_Stack.pop_front();

			std::lock_guard<std::mutex> lk{ m_WinMutex };
			for(auto& cb : m_WinHandlers)
			{
				(cb.Target->*cb.Func)(e);
			}
		}
	}

	void pio::EventMgr::DispatchEvent()
	{
		std::lock_guard<std::mutex> lk{ m_SlotEvtMutex };
		while(!m_SlotEvents.empty())
		{
			auto e = m_SlotEvents.front();
			m_SlotEvents.pop_front();

			std::lock_guard<std::mutex> lk{ m_MapMutex };
			auto it = m_SlotMap.find(e->Id);
			if(it != m_SlotMap.end())
			{
				auto& callbackList = it->second;
				for(auto& cb : callbackList)
				{
					(cb.Target->*cb.Func)(e);
				}
			}
		}
	}
}