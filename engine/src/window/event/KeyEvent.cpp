#include "KeyEvent.h"

namespace pio
{
	template<>
	bool Event::is<KeyPressedEvent>() { return getEventType() == EventType::KeyPressed; }

	template<>
	bool Event::is<KeyReleasedEvent>() { return getEventType() == EventType::KeyReleased; }

	template<>
	bool Event::is<KeyTypedEvent>() { return getEventType() == EventType::KeyTyped; }
}