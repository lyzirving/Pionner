#ifndef __PIONNER_EVENT_CURSOR_H__
#define __PIONNER_EVENT_CURSOR_H__

#include "Entry.h"

namespace pio
{
	enum class CursorMode : uint8_t
	{
		Normal = 0, Hidden, Disabled
	};
}

#endif