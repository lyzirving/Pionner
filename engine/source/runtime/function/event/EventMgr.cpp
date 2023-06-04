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
		, m_windowWidth(0), m_windowHeight(0)
		, m_pressAndMoving(false)
	{
	}

	EventMgr::~EventMgr()
	{
	}

	void EventMgr::processEvent()
	{
		if (m_curEvt.m_action == ACTION_BTN_DOWN && m_pressAndMoving)
		{
			LOG_DEBUG("pressed and moving");
		}
		else if (m_curEvt.m_action == ACTION_BTN_UP && m_lastEvt.m_action == ACTION_BTN_DOWN)
		{
			m_lastEvt = m_curEvt;
			if (m_pressAndMoving)
			{
				// finish press and move, no event will generate
				m_pressAndMoving = false;
			}
			else
			{
				LOG_DEBUG("click event");
			}
		}
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
}