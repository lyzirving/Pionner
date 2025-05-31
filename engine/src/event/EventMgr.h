#pragma once

#include "EventSocket.h"

namespace pio
{
	/*
	* @brief  EventMgr has 2 main function:
	*         One is used to collect Window event chain in window's work thread,
	*         and dispatch these events in logic thread.
	*		  The other is to disptach custom event.	
	*/
	class EventMgr
	{
		PIO_SINGLETON_DECLARE(EventMgr)
	public:
		// ------- API for win event chain ------------		
		void PushBackWin(Ref<Event> &event);
		void ConnectWin(EventCallback &&cb);
		void RemoveWin(EventCallback &&cb);
		// --------------------------------------------
		void BroadcastEvent(const Ref<Event>& event);
		void Dispatch();		

	private:
		void AddSlotCallback(uint32_t id, EventCallback&& cb);
		void RemoveSlotCallback(uint32_t id, EventCallback&& cb);

		void DispatchWinHandlers();
		void DispatchEvent();

	private:
		std::mutex m_StackMutex, m_WinMutex;
		std::list<Ref<Event>> m_Stack;
		std::vector<EventCallback> m_WinHandlers;

		std::mutex m_SlotEvtMutex, m_MapMutex;
		std::list<Ref<Event>> m_SlotEvents; 
		std::unordered_map<uint32_t, std::vector<EventCallback>> m_SlotMap;

	private:
		friend class EventSocket;
	};
}