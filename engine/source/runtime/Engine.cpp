#include "Engine.h"

#include "core/log/LogSystem.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Engine"

namespace Pionner
{
	Engine::Engine() {}

	Engine::~Engine() = default;

	void Engine::initialize()
	{
		LogSystem::initialize();

		LOG_DEBUG("finish initialization");
	}

	void Engine::destroy()
	{
		LogSystem::destroy();
	}
}