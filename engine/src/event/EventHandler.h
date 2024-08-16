#ifndef __PIONNER_EVENT_EVENT_HANDLER_H__
#define __PIONNER_EVENT_EVENT_HANDLER_H__

#include "Event.h"

namespace pio
{
	class EventHandler
	{
	public:
		virtual bool onMouseButtonPressed(Event &event) = 0;
		virtual bool onMouseButtonReleased(Event &event) = 0;
		virtual bool onMouseMoved(Event &event) = 0;
		virtual bool onMouseMoveHovering(Event &event) { return false; }
		virtual bool onMouseScrolled(Event &event) = 0;
	};
}

#endif