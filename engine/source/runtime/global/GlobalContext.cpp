#include "global/GlobalContext.h"
#include "window/WindowSystem.h"
#include "render/RenderSystem.h"

#include "world/World.h"
#include "event/EventMgr.h"

#include "world/PioWorld.h"

#include "core/log/LogSystem.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "GlobalContext"

namespace pio
{
	GlobalContext g_runtimeCtx{};

	GlobalContext::GlobalContext()
		: m_world(nullptr)
		, m_windowSystem(nullptr)
		, m_renderSystem(nullptr)
		, m_pioWorld(nullptr)
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

		m_pioWorld = std::make_shared<PioWorld>();
		m_pioWorld->init();
	}

	void GlobalContext::swapData(float deltaTime)
	{
		if (m_pioWorld->dirty())
		{
			m_pioWorld->setDirty(false);
		}
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

		if (m_pioWorld)
		{
			m_pioWorld->shutdown();
			m_pioWorld.reset();
		}

		LOG_DEBUG("all systems are shutdown");
		LogSystem::shutdown();
	}
}