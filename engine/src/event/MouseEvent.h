#pragma once

#include "Event.h"
#include "MouseCodes.h"

namespace pio 
{
	class MouseScrolledEvent : public Event
	{
		RTTR_ENABLE(Event)
	public:
		MouseScrolledEvent(const float xOffset, const float yOffset)
			: Event(), m_OffsetX(xOffset), m_OffsetY(yOffset) {}

		float OffsetX() const { return m_OffsetX; }
		float OffsetY() const { return m_OffsetY; }

		virtual std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseScrolledEvent: " << m_OffsetX << ", " << m_OffsetY;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseScrolled)
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

	private:
		float m_OffsetX, m_OffsetY;
	};
}
