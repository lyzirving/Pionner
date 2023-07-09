#include <limits>

#include "Engine.h"

#include "global/GlobalContext.h"
#include "global/window/WindowSystem.h"
#include "render/RenderSystem.h"

#include "scenegraph/view/Scene.h"

#include "core/system/TimeUtil.h"
#include "core/log/LogSystem.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Engine"

namespace pio
{
	Engine::Engine()
	{
	}

	Engine::~Engine() = default;

	void Engine::initialize()
	{
		g_runtimeCtx.startSystems();
	}

	void Engine::shutdown()
	{
		g_runtimeCtx.shutdownSystems();
	}

	uint64_t Engine::calculateDeltaTime()
	{
		uint64_t delta{ 0 };
		uint64_t curTimeMs = TimeUtil::currentTimeMs();
		if (m_lastTimeMs != 0)
		{
			delta = curTimeMs - m_lastTimeMs;
		}
		m_lastTimeMs = curTimeMs;
		return delta;
	}

	bool Engine::tickFrame(uint64_t deltaMs)
	{
		tickLogic(deltaMs);

		g_runtimeCtx.m_windowSystem->makeCurrent();

		tickRender(deltaMs);

		g_runtimeCtx.m_windowSystem->swapBuffers();

		g_runtimeCtx.m_windowSystem->pollEvents();

		return !g_runtimeCtx.m_windowSystem->shouldClose();
	}

	void Engine::tickLogic(uint64_t deltaMs)
	{
		g_runtimeCtx.swapData(deltaMs);

		g_runtimeCtx.m_renderSystem->swapData(deltaMs);
	}

	void Engine::tickRender(uint64_t deltaMs)
	{
		g_runtimeCtx.m_renderSystem->tick(deltaMs);

		g_runtimeCtx.m_scene->tick(deltaMs);
	}
}