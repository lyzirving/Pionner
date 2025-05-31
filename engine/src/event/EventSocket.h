#pragma once

#include "Event.h"

namespace pio
{
	struct EventCallback;

	class EventSocket
	{
	public:
		EventSocket() {}
		virtual ~EventSocket() = default;

		void AddSlotCallback(uint32_t id, EventCallback&& cb);
		void RemoveSlotCallback(uint32_t id, EventCallback&& cb);
	};

	using SLOTFUNCTION = void(EventSocket::*)(const Ref<Event>&);

	struct EventCallback
	{
		EventSocket* Target{ nullptr };
		SLOTFUNCTION Func{ nullptr };

		EventCallback() {}
		EventCallback(EventSocket* t, SLOTFUNCTION fun) : Target(t), Func(fun) {}

		bool operator==(const EventCallback& rhs) const { return this->Target == rhs.Target && this->Func == rhs.Func; }
		bool operator!=(const EventCallback& rhs) const { return !((*this) == rhs); }
	};
}