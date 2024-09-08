#include "KeyEvent.h"

namespace pio
{
	template<>
	bool Event::is<KeyPressedEvent>() const { return getEventType() == EventType::KeyPressed; }

	template<>
	bool Event::is<KeyReleasedEvent>() const { return getEventType() == EventType::KeyReleased; }

	template<>
	bool Event::is<KeyTypedEvent>() const { return getEventType() == EventType::KeyTyped; }
}