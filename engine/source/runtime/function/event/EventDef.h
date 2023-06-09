#ifndef __EVENT_DEF_H__
#define __EVENT_DEF_H__

#include <cstdint>

namespace Pionner
{
	static const float INVALID_CURSOR_POS = -1.f;

	enum CursorButton
	{
		BTN_LEFT,
		BTN_RIGHT
	};

	enum EventAction
	{
		ACTION_BTN_UP,
		ACTION_BTN_DOWN
	};

	enum EventType : uint8_t
	{
		EVENT_TYPE_NONE,
		EVENT_TYPE_CLICK,
		EVENT_TYPE_PRESSED_MOVING,
		EVENT_TYPE_PRESSED_MOVING_FINISH,
		EVENT_TYPE_SCROLLING,
		EVENT_TYPE_COUNT
	};
}

#endif