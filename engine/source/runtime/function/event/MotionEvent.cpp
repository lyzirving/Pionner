#include "function/event/MotionEvent.h"

namespace Pionner
{
	MotionEvent::MotionEvent()
		: m_button(-1), m_action(-1), m_mods(-1)
	{
	}

	MotionEvent::MotionEvent(int button, int action, int mods)
		: m_button(button), m_action(action), m_mods(mods)
	{
	}

	MotionEvent::~MotionEvent()
	{
	}
}