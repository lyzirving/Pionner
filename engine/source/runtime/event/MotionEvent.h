#ifndef __MOTION_EVENT_H__
#define __MOTION_EVENT_H__

namespace pio
{
	class MotionEvent
	{
	public:
		MotionEvent();
		MotionEvent(int button, int action, int mods);
		~MotionEvent();

	public:
		int m_button, m_action, m_mods;
	};
}

#endif