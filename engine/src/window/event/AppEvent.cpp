#include "AppEvent.h"

namespace pio
{
	template<>
	bool Event::is<WindowResizeEvent>() { return getEventType() == EventType::WindowResize; }

	template<>
	bool Event::is<WindowCloseEvent>() { return getEventType() == EventType::WindowClose; }

	template<>
	bool Event::is<AppTickEvent>() { return getEventType() == EventType::AppTick; }

	template<>
	bool Event::is<AppUpdateEvent>() { return getEventType() == EventType::AppUpdate; }

	template<>
	bool Event::is<AppRenderEvent>() { return getEventType() == EventType::AppRender; }
}