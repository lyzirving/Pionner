#ifndef __PIONNER_GLOBAL_DEF_H__
#define __PIONNER_GLOBAL_DEF_H__

#include <string>

namespace pio
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

	enum PioEntityType : uint8_t
	{
		PIO_ENTITY_GEO,
		PIO_ENTITY_CNT
	};

	struct EntityParam
	{
		PioEntityType type{ PIO_ENTITY_CNT };
		std::string   nodeName{};
	};

	struct WindowSystemInitInfo
	{
		int         width{ 1600 };
		int         height{ 900 };
		const char *title{ "Pionner" };
		bool        fullscreen{ false };
	};
}

#endif