#include "global/GlobalContext.h"
#include "global/window/WindowSystem.h"
#include "render/RenderSystem.h"

#include "world/World.h"
#include "global/event/EventMgr.h"

#include "PioWorld.h"
#include "render2/Render.h"

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

		m_render = std::make_shared<render::Render>("Main Render");
	}

	void GlobalContext::swapData(uint64_t deltaMs)
	{
		if (m_render->emptyScene())
		{
			m_render->setScene(m_pioWorld->getScene());
		}
		m_pioWorld->swap(deltaMs);
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

		if (m_render)
		{
			m_render->release();
			m_render.reset();
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