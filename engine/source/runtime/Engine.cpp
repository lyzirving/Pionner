#include <limits>

#include "Engine.h"

#include "function/global/GlobalContext.h"
#include "function/render/WindowSystem.h"
#include "function/render/RenderSystem.h"

#include "core/log/LogSystem.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Engine"

namespace Pionner
{
	const float Engine::s_fpsAlpha = 1.f / 100.f;

	Engine::Engine() : m_lastTickTimePoint(std::chrono::steady_clock::now())
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

	float Engine::calculateDeltaTime()
	{
		float deltaTime{ 0.f };
		{
			using namespace std::chrono;

			steady_clock::time_point tickPoint = steady_clock::now();
			duration<float> timeSpan = duration_cast<duration<float>>(tickPoint - m_lastTickTimePoint);
			deltaTime = timeSpan.count();

			m_lastTickTimePoint = tickPoint;
		}
		return deltaTime;
	}

	bool Engine::tickFrame(float deltaTime)
	{
		tickLogic(deltaTime);

		tickRender(deltaTime);

		g_runtimeCtx.m_windowSystem->swapBuffers();

		g_runtimeCtx.m_windowSystem->pollEvents();

		return !g_runtimeCtx.m_windowSystem->shouldClose();
	}

	void Engine::tickLogic(float deltaTime)
	{

	}

	void Engine::tickRender(float deltaTime)
	{
		g_runtimeCtx.m_renderSystem->tick(deltaTime);
	}
}