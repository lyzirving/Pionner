#ifndef __PIONNER_WINDOW_EVENT_MOUSE_EVENT_H__
#define __PIONNER_WINDOW_EVENT_MOUSE_EVENT_H__

#include "Event.h"
#include "window/input/MouseCodes.h"

namespace pio 
{

	class MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(const float x, const float y)
			: m_mouseX(x), m_mouseY(y) {}

		inline float getX() const { return m_mouseX; }
		inline float getY() const { return m_mouseY; }

		virtual std::string toString() const override
		{
			std::stringstream ss;
			ss << "MouseMovedEvent: " << m_mouseX << ", " << m_mouseY;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseMoved)
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

	private:
		float m_mouseX, m_mouseY;
	};

	class MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent(const float xOffset, const float yOffset)
			: m_offsetX(xOffset), m_offsetY(yOffset) {}

		inline float getXOffset() const { return m_offsetX; }
		inline float getYOffset() const { return m_offsetY; }

		virtual std::string toString() const override
		{
			std::stringstream ss;
			ss << "MouseScrolledEvent: " << m_offsetX << ", " << m_offsetY;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseScrolled)
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

	private:
		float m_offsetX, m_offsetY;
	};

	class MouseButtonEvent : public Event
	{
	public:
		MouseCode getMouseButton() const { return m_button; }
		float getCursorX() const { return m_cursorX; }
		float getCursorY() const { return m_cursorY; }

		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput | EventCategoryMouseButton)

	protected:
		MouseButtonEvent(const MouseCode button, float x, float y) : m_button(button), m_cursorX(x), m_cursorY(y) {}

	protected:
		MouseCode m_button;
		float m_cursorX, m_cursorY;
	};

	class MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(const MouseCode button, float x = 0.f, float y = 0.f)
			: MouseButtonEvent(button, x, y) {}

		virtual std::string toString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonPressedEvent: " << m_button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonPressed)
	};

	class MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(const MouseCode button, float x = 0.f, float y = 0.f)
			: MouseButtonEvent(button, x, y) {}

		virtual std::string toString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonReleasedEvent: " << m_button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonReleased)
	};

	template<>
	bool Event::is<MouseMovedEvent>();

	template<>
	bool Event::is<MouseScrolledEvent>();

	template<>
	bool Event::is<MouseButtonPressedEvent>();

	template<>
	bool Event::is<MouseButtonReleasedEvent>();

}

#endif
