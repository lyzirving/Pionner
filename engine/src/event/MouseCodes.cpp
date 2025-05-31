#include "MouseCodes.h"

namespace pio
{
	std::atomic<MouseCode> Mouse::k_MouseCode{ Mouse::ButtonRelease };
	std::atomic<float> Mouse::k_PosX{ 0.f };
	std::atomic<float> Mouse::k_PosY{ 0.f };
}