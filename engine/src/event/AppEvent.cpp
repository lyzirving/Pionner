#include "AppEvent.h"

namespace pio
{
	template<>
	bool Event::is<WindowResizeEvent>() const { return getEventType() == EventType::WindowResize; }

	template<>
	bool Event::is<WindowCloseEvent>() const { return getEventType() == EventType::WindowClose; }

	template<>
	bool Event::is<AppTickEvent>() const { return getEventType() == EventType::AppTick; }

	template<>
	bool Event::is<AppUpdateEvent>() const { return getEventType() == EventType::AppUpdate; }

	template<>
	bool Event::is<AppRenderEvent>() const { return getEventType() == EventType::AppRender; }
}