#ifndef __EVENT_MGR_H__
#define __EVENT_MGR_H__

#include "function/event/MotionEvent.h"

namespace Pionner
{
	class EventMgr
	{
	public:
		EventMgr();
		~EventMgr();

		void setCursorPos(double x, double y);
		void processEvent();

		inline double getCursorPosX() { return m_cursorPosX; }
		inline double getCursorPosY() { return m_cursorPosY; }

		inline void setWindowSize(int width, int height)
		{
			m_windowWidth = width;
			m_windowHeight = height;
		}

		inline void setMotionEvent(int button, int action, int mods)
		{
			m_lastEvt = m_curEvt;
			m_curEvt = MotionEvent(button, action, mods);
		}

	private:
		MotionEvent m_curEvt, m_lastEvt;

		double m_cursorPosX, m_cursorPosY;
		double m_lastCursorPosX, m_lastCursorPosY;

		int    m_windowWidth, m_windowHeight;
		bool   m_pressAndMoving;
	};
}

#endif