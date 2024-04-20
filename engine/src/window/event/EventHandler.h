#ifndef __PIONNER_WINDOW_EVENT_EVENT_HANDLER_H__
#define __PIONNER_WINDOW_EVENT_EVENT_HANDLER_H__

#include "Event.h"

namespace pio
{
	class EventHandler
	{
	public:
		virtual bool onMouseButtonPressed(Event &event) = 0;
		virtual bool onMouseButtonReleased(Event &event) = 0;
		virtual bool onMouseMoved(Event &event) = 0;
		virtual bool onMouseScrolled(Event &event) = 0;
	};
}

#endif