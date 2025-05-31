#include "EventSocket.h"
#include "EventMgr.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "EventSocket"

namespace pio
{
	void EventSocket::AddSlotCallback(uint32_t id, EventCallback&& cb)
	{
		EventMgr::Get()->AddSlotCallback(id, std::forward<EventCallback>(cb));
	}

	void EventSocket::RemoveSlotCallback(uint32_t id, EventCallback&& cb)
	{
		EventMgr::Get()->RemoveSlotCallback(id, std::forward<EventCallback>(cb));
	}
}