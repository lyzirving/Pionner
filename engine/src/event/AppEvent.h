#pragma once

#include "Event.h"

namespace pio 
{

	class WindowResizeEvent : public Event
	{
		RTTR_ENABLE(Event)
	public:
		WindowResizeEvent(uint32_t width, uint32_t height) : m_Width(width), m_Height(height) {}

		uint32_t Width() const { return m_Width; }
		uint32_t Height() const { return m_Height; }

		virtual std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowResizeEvent: " << m_Width << ", " << m_Height;
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowResize)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)

	private:
		uint32_t m_Width, m_Height;
	};

	class WindowCloseEvent : public Event
	{
		RTTR_ENABLE(Event)
	public:
		WindowCloseEvent() = default;

		EVENT_CLASS_TYPE(WindowClose)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};
}