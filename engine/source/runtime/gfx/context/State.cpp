#include "State.h"

namespace pio
{
	namespace gfx
	{
		State *State::    g_instance{ nullptr };
		std::mutex State::g_objMutex{};

		State::State()
		{
		}

		State::~State() = default;

		State *State::self()
		{
			if (!g_instance)
			{
				std::lock_guard<std::mutex> guard{ g_objMutex };
				if (!g_instance)
				{
					g_instance = new State;
				}
			}
			return g_instance;
		}
	}
}