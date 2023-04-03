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
					 , m_averageDuration(0.f)
					 , m_frameCnt(0), m_fps(0)
	{
	}

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

	void Engine::calculateFps(float deltaTime)
	{
		++m_frameCnt;
		if (m_frameCnt == INT_MAX - 1)
			m_frameCnt = 1;

		if (m_frameCnt == 1)
		{
			m_averageDuration = deltaTime;
		}
		else
		{
			m_averageDuration = m_averageDuration * (1 - s_fpsAlpha) + deltaTime * s_fpsAlpha;
		}

		m_fps = static_cast<int>(1.f / m_averageDuration);
	}

	float Engine::calculateDeltaTime()
	{
		float deltaTime{0.f};
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

		calculateFps(deltaTime);

		// TODO: swap logic and render data

		tickRender(deltaTime);

		g_runtimeCtx.m_windowSystem->swapBuffers();

		g_runtimeCtx.m_windowSystem->pollEvents();

		return !g_runtimeCtx.m_windowSystem->shouldClose();
	}

	void Engine::tickLogic(float deltaTime)
	{
		// TODO: to be implemented
	}

	void Engine::tickRender(float deltaTime)
	{
		g_runtimeCtx.m_renderSystem->tick(deltaTime);
	}
}