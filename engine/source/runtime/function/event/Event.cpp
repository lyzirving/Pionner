#include <utility>

#include "function/event/Event.h"

namespace Pionner
{
	Event::Event() noexcept
		: m_posX(0), m_posY(0)
		, m_scrollDeltaX(0), m_scrollDeltaY(0)
		, m_type(EVENT_TYPE_NONE)
	{
	}

	Event::~Event() = default;

	Event::Event(const Event &evt) noexcept
		: m_posX(evt.m_posX), m_posY(evt.m_posY)
		, m_scrollDeltaX(evt.m_scrollDeltaX), m_scrollDeltaY(evt.m_scrollDeltaY)
		, m_type(evt.m_type)
	{
	}

	Event::Event(Event &&evt) noexcept
		: m_posX(std::move(evt.m_posX)), m_posY(std::move(evt.m_posY))
		, m_scrollDeltaX(std::move(evt.m_scrollDeltaX))
		, m_scrollDeltaY(std::move(evt.m_scrollDeltaY))
		, m_type(std::move(evt.m_type))
	{
	}

	Event &Event::operator=(const Event &evt) noexcept
	{
		if (this != &evt)
		{
			m_posX = evt.m_posX;
			m_posY = evt.m_posY;
			m_scrollDeltaX = evt.m_scrollDeltaX;
			m_scrollDeltaY = evt.m_scrollDeltaY;
			m_type = evt.m_type;
		}
		return *this;
	}

	Event &Event::operator=(Event &&evt) noexcept
	{
		if (this != &evt)
		{
			m_posX = std::move(evt.m_posX);
			m_posY = std::move(evt.m_posY);
			m_scrollDeltaX = std::move(evt.m_scrollDeltaX);
			m_scrollDeltaY = std::move(evt.m_scrollDeltaY);
			m_type = std::move(evt.m_type);
		}
		return *this;
	}
}