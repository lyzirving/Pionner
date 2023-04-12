#include "function/framework/event/EventSystem.h"
#include "function/framework/object/GameObject.h"

namespace Pionner
{
	EventSystem::EventSystem() : m_events()
	{
	}

	EventSystem::~EventSystem()
	{
		clear();
	}

	void EventSystem::addEvent(const std::shared_ptr<GameObject>& obj, const std::shared_ptr<EventSlot>& slot)
	{
	}

	void EventSystem::clear()
	{
		auto itr = m_events.begin();
		while (itr != m_events.end())
		{
			(*itr).first.reset();
			(*itr).second.reset();
			itr = m_events.erase(itr);
		}
	}

	void EventSystem::tick()
	{
		while (!m_events.empty())
		{
			auto &evt = m_events.back();
			std::shared_ptr<GameObject>& object = evt.first;
			std::shared_ptr<EventSlot>& slot = evt.second;
			object->tickLogicEvent(slot);
			object.reset();
			slot.reset();
			m_events.pop_back();
		}
	}
}
