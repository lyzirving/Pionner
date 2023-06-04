#include "function/global/GlobalContext.h"
#include "function/render/WindowSystem.h"
#include "function/render/RenderSystem.h"

#include "function/framework/world/World.h"
#include "function/event/EventMgr.h"

#include "core/log/LogSystem.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "GlobalContext"

namespace Pionner
{
	GlobalContext g_runtimeCtx{};

	GlobalContext::GlobalContext()
		: m_world(nullptr)
		, m_windowSystem(nullptr)
		, m_renderSystem(nullptr)
	{
	}

	GlobalContext::~GlobalContext() = default;

	void GlobalContext::startSystems()
	{
		LogSystem::initialize();

		m_world = std::make_shared<World>();

		std::shared_ptr<EventMgr> evtMgr = std::make_shared<EventMgr>();

		WindowSystemInitInfo windowInitInfo;
		m_windowSystem = std::make_shared<WindowSystem>(m_world);
		m_windowSystem->initialize(windowInitInfo);

		RenderSystemInitInfo renderInitInfo;
		renderInitInfo.window = m_windowSystem;
		m_renderSystem = std::make_shared<RenderSystem>(m_world);
		m_renderSystem->initialize(renderInitInfo);

		m_world->build();
	}

	void GlobalContext::shutdownSystems()
	{
		if (m_renderSystem)
		{
			m_renderSystem->shutdown();
			m_renderSystem.reset();
		}

		if (m_windowSystem)
		{
			m_windowSystem->shutdown();
			m_windowSystem.reset();
		}

		if (m_world)
		{
			m_world->shutdown();
			m_world.reset();
		}

		LOG_DEBUG("all systems are shutdown");
		LogSystem::shutdown();
	}
}