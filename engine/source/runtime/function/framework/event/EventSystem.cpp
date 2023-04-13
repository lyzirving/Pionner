#include "function/framework/event/EventSystem.h"
#include "function/framework/object/GameObject.h"

namespace Pionner
{
	EventSystem::EventSystem() : m_logicEvents()
		                       , m_tickingLogic(false)
	{
	}

	EventSystem::~EventSystem()
	{
		clear();
	}

	void EventSystem::addEvent(const std::shared_ptr<GameObject>& obj, const std::shared_ptr<EventSlot>& slot)
	{
		if (m_tickingLogic.load())
		{

		}
		else
		{
			SLOT logicEvent = std::pair<std::shared_ptr<GameObject>, std::shared_ptr<EventSlot>>{ obj, slot };
			m_logicEvents.push_back(std::move(logicEvent));
		}
	}

	void EventSystem::clear()
	{
		auto itr = m_logicEvents.begin();
		while (itr != m_logicEvents.end())
		{
			(*itr).first.reset();
			(*itr).second.reset();
			itr = m_logicEvents.erase(itr);
		}
	}

	void EventSystem::tickLogic(float delta)
	{
		m_tickingLogic.store(true);
		while (!m_logicEvents.empty())
		{
			auto &evt = m_logicEvents.back();
			std::shared_ptr<GameObject>& object = evt.first;
			std::shared_ptr<EventSlot>& slot = evt.second;
			object->tickLogicEvent(slot);
			object.reset();
			slot.reset();
			m_logicEvents.pop_back();
		}
		m_tickingLogic.store(false);
	}
}
