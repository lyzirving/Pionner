#include "function/event/EventMgr.h"
#include "function/event/EventDef.h"

#include "core/log/LogSystem.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "EventMgr"

namespace Pionner
{
	Pionner::EventMgr::EventMgr()
		: m_curEvt(), m_lastEvt()
		, m_cursorPosX(0), m_cursorPosY(0)
		, m_lastCursorPosX(0), m_lastCursorPosY(0)
		, m_curScrollDeltaX(0), m_curScrollDeltaY(0)
		, m_scrollDiffX(0), m_scrollDiffY(0)
		, m_windowWidth(0), m_windowHeight(0)
		, m_pressAndMoving(false)
		, m_scrolling(false)
	{
	}

	EventMgr::~EventMgr()
	{
	}

	Event EventMgr::processEvent()
	{
		Event evt;
		if (m_scrolling)
		{
			//LOG_DEBUG("scrolling, delta[%lf, %lf]", m_curScrollDeltaX, m_curScrollDeltaY);
			m_scrolling = false;
			evt.m_type = EVENT_TYPE_SCROLLING;
			evt.m_scrollDeltaX = m_curScrollDeltaX;
			evt.m_scrollDeltaY = m_curScrollDeltaY;
			evt.m_posX = m_cursorPosX;
			evt.m_posY = m_cursorPosY;
		}
		else if (m_curEvt.m_action == ACTION_BTN_DOWN && m_pressAndMoving)
		{
			//LOG_DEBUG("pressed and moving, cur position[%lf, %lf]", m_cursorPosX, m_cursorPosY);
			evt.m_type = EVENT_TYPE_PRESSED_MOVING;
			evt.m_posX = m_cursorPosX;
			evt.m_posY = m_cursorPosY;
		}
		else if (m_curEvt.m_action == ACTION_BTN_UP && m_lastEvt.m_action == ACTION_BTN_DOWN)
		{
			m_lastEvt = m_curEvt;
			if (m_pressAndMoving)
			{
				// finish press and move, no event will generate
				m_pressAndMoving = false;
				evt.m_type = EVENT_TYPE_PRESSED_MOVING_FINISH;
			}
			else
			{
				//LOG_DEBUG("click event, cur position[%lf, %lf]", m_cursorPosX, m_cursorPosY);
				evt.m_type = EVENT_TYPE_CLICK;
			}
			evt.m_posX = m_cursorPosX;
			evt.m_posY = m_cursorPosY;
		}
		return evt;
	}

	void EventMgr::setCursorPos(double x, double y)
	{
		m_lastCursorPosX = m_cursorPosX;
		m_lastCursorPosY = m_cursorPosY;

		m_cursorPosX = x;
		m_cursorPosY = y;

		if (m_curEvt.m_action == ACTION_BTN_DOWN)
		{
			m_pressAndMoving = true;
		}
	}

	void EventMgr::setScroll(double deltaX, double deltaY)
	{
		m_curScrollDeltaX = deltaX;
		m_curScrollDeltaY = deltaY;

		m_scrollDiffX += deltaX;
		m_scrollDiffY += deltaY;

		m_scrolling = true;
	}
}