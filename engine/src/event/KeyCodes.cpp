#include "KeyCodes.h"

namespace pio
{
	std::atomic<KeyCode> Key::k_KeyCode{ Key::None };
}