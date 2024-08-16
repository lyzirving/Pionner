#ifndef __PIONNER_EVENT_EVENT_HUB_H__
#define __PIONNER_EVENT_EVENT_HUB_H__

#include "Event.h"

namespace pio
{
	class EventHubObj
	{
	public:
		EventHubObj() {}
		virtual ~EventHubObj() = default;
	};

	using EventHubCbFun = void(EventHubObj:: *)(Ref<Event> &);

	struct EventHubCb
	{
		EventHubObj *Target{ nullptr };
		EventHubCbFun Cb{ nullptr };

		EventHubCb() {}
		EventHubCb(EventHubObj *t, EventHubCbFun fun) : Target(t), Cb(fun) {}

		bool operator==(const EventHubCb &rhs) const
		{
			return this->Target == rhs.Target && this->Cb == rhs.Cb;
		}
	};

	/*
	* @brief  EventHub is used to collect Window events in window's work thread,
	*         and dispatch these events in logic thread.
	*/
	class EventHub
	{
		PIO_SINGLETON_DECLARE(EventHub)
	public:
		void add(Ref<Event> &event);
		void registerCallback(EventHubCb &&cb);
		void removeCallback(EventHubCb &&cb);

		void dispatch();

	private:
		void dispatchToCallbackList(Ref<Event> &event);

	private:
		std::mutex m_stackMutex, m_cbMutex;
		std::list<Ref<Event>> m_stack;
		std::vector<EventHubCb> m_cbList;
	};
}

#endif