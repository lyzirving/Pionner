#include "function/framework/event/EventSystem.h"
#include "function/framework/object/GameObject.h"

namespace Pionner
{
	EventSystem::EventSystem() : m_logicEvents()
		                       , m_tmpEvents()
		                       , m_tickingLogic(false)
	{
	}

	EventSystem::~EventSystem()
	{
		clear();
	}

	void EventSystem::addEvent(const std::shared_ptr<GameObject>& obj, const std::shared_ptr<EventSlot>& slot)
	{
		SLOT logicEvent = std::pair<std::shared_ptr<GameObject>, std::shared_ptr<EventSlot>>{ obj, slot };
		if (m_tickingLogic.load())
		{
			m_tmpEvents.push_back(std::move(logicEvent));
		}
		else
		{
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

		auto itrTmp = m_tmpEvents.begin();
		while (itrTmp != m_tmpEvents.end())
		{
			(*itrTmp).first.reset();
			(*itrTmp).second.reset();
			itrTmp = m_tmpEvents.erase(itrTmp);
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
			object->tickLogicEvent(delta, slot);
			object.reset();
			slot.reset();
			m_logicEvents.pop_back();
		}
		m_tickingLogic.store(false);

		// check events added during ticking
		if (!m_tmpEvents.empty())
		{
			m_logicEvents.insert(m_logicEvents.end(),
				                 m_tmpEvents.begin(), 
				                 m_tmpEvents.end());
			std::vector<SLOT>().swap(m_tmpEvents);
		}
	}
}
