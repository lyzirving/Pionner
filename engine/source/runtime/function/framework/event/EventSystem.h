#ifndef __EVENT_SYSTEM_H__
#define __EVENT_SYSTEM_H__

#include <vector>
#include <atomic>
#include <memory>

#include "function/framework/event/EventDef.h"

namespace Pionner
{
	class GameObject;

	class EventSystem
	{
	public:
		using SLOT = std::pair<std::shared_ptr<GameObject>, std::shared_ptr<EventSlot>>;

		EventSystem();
		~EventSystem();

		void addEvent(const std::shared_ptr<GameObject>& obj, const std::shared_ptr<EventSlot> &slot);
		void clear();
		void tickLogic(float delta);

	private:
		std::vector<SLOT> m_logicEvents;
		std::vector<SLOT> m_tmpEvents;
		std::atomic<bool> m_tickingLogic;
	};
}

#endif