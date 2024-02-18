#ifndef __PIONNER_WINDOW_EVENT_KEY_EVENT_H__
#define __PIONNER_WINDOW_EVENT_KEY_EVENT_H__

#include "Event.h"
#include "window/input/KeyCodes.h"

namespace pio
{

	class KeyEvent : public Event
	{
	public:
		KeyCode getKeyCode() const { return m_keyCode; }

		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
	protected:
		KeyEvent(const KeyCode keycode) : m_keyCode(keycode) {}

	protected:
		KeyCode m_keyCode;
	};

	class KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(const KeyCode keycode, bool isRepeat = false)
			: KeyEvent(keycode), m_isRepeat(isRepeat) {}

		bool IsRepeat() const { return m_isRepeat; }

		virtual std::string toString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << m_keyCode << " (repeat = " << m_isRepeat << ")";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed)

	private:
		bool m_isRepeat;
	};

	class KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(const KeyCode keycode) : KeyEvent(keycode) {}

		virtual std::string toString() const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << m_keyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased)
	};

	class KeyTypedEvent : public KeyEvent
	{
	public:
		KeyTypedEvent(const KeyCode keycode) : KeyEvent(keycode) {}

		virtual std::string toString() const override
		{
			std::stringstream ss;
			ss << "KeyTypedEvent: " << m_keyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyTyped)
	};

	template<>
	bool Event::is<KeyPressedEvent>();

	template<>
	bool Event::is<KeyReleasedEvent>();

	template<>
	bool Event::is<KeyTypedEvent>();
}


#endif