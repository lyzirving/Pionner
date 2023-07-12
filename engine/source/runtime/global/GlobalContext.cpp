#include "global/GlobalContext.h"
#include "global/window/WindowSystem.h"
#include "render/RenderSystem.h"

#include "world/World.h"

#include "global/event/EventMgr.h"

#include "PioWorld.h"

#include "render2/RenderSystem.h"
#include "scenegraph/view/Scene.h"
#include "gfx/context/GraphicContext.h"

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

		/*m_world = std::make_shared<World>();
		RenderSystemInitInfo renderInitInfo;
		renderInitInfo.window = m_windowSystem;
		m_renderSystem = std::make_shared<RenderSystem>(m_world);
		m_renderSystem->initialize(renderInitInfo);
		m_world->build();*/

		// window will initialize glfw which create a rendering context.
		WindowSystemInitInfo windowInitInfo;
		m_windowSystem = std::make_shared<WindowSystem>();
		m_windowSystem->initialize(windowInitInfo);

		// gfx context will initialize glew which relies on a rendering context.
		m_gfxContext = std::make_shared<gfx::GraphicContext>();
		m_gfxContext->init();

		m_render = std::make_shared<render::RenderSystem>(m_gfxContext);

		m_pioWorld = std::make_shared<PioWorld>();
		// attach() must be called before init()
		m_pioWorld->attach(m_render->getScene());
		m_pioWorld->init();
	}

	void GlobalContext::swapData(uint64_t deltaMs)
	{
		auto eventMgr = m_windowSystem->getEvtMgr();
		Event event = eventMgr->processEvent();

		m_pioWorld->swap(deltaMs);
		m_render->setWndSize(m_windowSystem->getWidth(), m_windowSystem->getHeight());
		m_render->dispatchEvent(event);
	}

	void GlobalContext::shutdownSystems()
	{
		if (m_renderSystem)
		{
			m_renderSystem->shutdown();
			m_renderSystem.reset();
		}

		if (m_world)
		{
			m_world->shutdown();
			m_world.reset();
		}

		if (m_windowSystem)
		{
			m_windowSystem->shutdown();
			m_windowSystem.reset();
		}

		if (m_pioWorld)
		{
			m_pioWorld->shutdown();
			m_pioWorld.reset();
		}

		if (m_render)
		{
			m_render->shutdown();
			m_render.reset();
		}

		if (m_gfxContext)
		{
			m_gfxContext->shutdown();
			m_gfxContext.reset();
		}

		LOG_DEBUG("all systems are shutdown");
		LogSystem::shutdown();
	}
}