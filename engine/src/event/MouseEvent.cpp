#include "MouseEvent.h"

namespace pio
{
	template<>
	bool Event::is<MouseMovedEvent>() const { return getEventType() == EventType::MouseMoved; }

	template<>
	bool Event::is<MouseScrolledEvent>() const { return getEventType() == EventType::MouseScrolled; }

	template<>
	bool Event::is<MouseButtonPressedEvent>() const { return getEventType() == EventType::MouseButtonPressed; }

	template<>
	bool Event::is<MouseButtonReleasedEvent>() const { return getEventType() == EventType::MouseButtonReleased; }
}