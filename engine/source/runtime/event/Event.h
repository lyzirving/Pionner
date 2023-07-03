#ifndef __EVENT_H__
#define __EVENT_H__

#include "EventDef.h"

namespace pio
{
	class Event
	{
	public:
		Event() noexcept;
		~Event();

		Event(const Event &evt) noexcept;
		Event(Event &&evt) noexcept;

		Event &operator=(const Event &evt) noexcept;
		Event &operator=(Event &&evt) noexcept;

	public:
		double m_posX, m_posY;
		double m_scrollDeltaX, m_scrollDeltaY;
		EventType m_type;
	};
}

#endif