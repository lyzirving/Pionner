#include "Engine.h"

#include "function/global/GlobalContext.h"
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
		g_runtimeCtx.startSystems();
		LOG_DEBUG("finish initialization");
	}

	void Engine::shutdown()
	{
		g_runtimeCtx.shutdownSystems();
	}
}