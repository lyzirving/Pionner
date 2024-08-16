#include "MouseEvent.h"

namespace pio
{
	template<>
	bool Event::is<MouseMovedEvent>() { return getEventType() == EventType::MouseMoved; }

	template<>
	bool Event::is<MouseScrolledEvent>() { return getEventType() == EventType::MouseScrolled; }

	template<>
	bool Event::is<MouseButtonPressedEvent>() { return getEventType() == EventType::MouseButtonPressed; }

	template<>
	bool Event::is<MouseButtonReleasedEvent>() { return getEventType() == EventType::MouseButtonReleased; }
}