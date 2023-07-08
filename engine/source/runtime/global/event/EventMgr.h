#ifndef __EVENT_MGR_H__
#define __EVENT_MGR_H__

#include "Event.h"
#include "MotionEvent.h"

namespace pio
{
	class EventMgr
	{
	public:
		EventMgr();
		~EventMgr();

		void  setCursorPos(double x, double y);
		void  setScroll(double deltaX, double deltaY);
		Event processEvent();

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

		double m_curScrollDeltaX, m_curScrollDeltaY;
		double m_scrollDiffX, m_scrollDiffY;

		int    m_windowWidth, m_windowHeight;
		bool   m_pressAndMoving;
		bool   m_scrolling;
	};
}

#endif