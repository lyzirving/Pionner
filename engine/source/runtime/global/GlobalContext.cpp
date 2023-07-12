#include "GlobalContext.h"
#include "PioWorld.h"

#include "global/event/EventMgr.h"
#include "global/window/WindowSystem.h"

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

	GlobalContext::GlobalContext() {}

	GlobalContext::~GlobalContext() = default;

	void GlobalContext::startSystems()
	{
		LogSystem::initialize();

		// window will initialize glfw which create a rendering context.
		WindowSystemInitInfo windowInitInfo;

		m_eventMgr = std::make_shared<EventMgr>();
		m_eventMgr->setWindowSize(windowInitInfo.width, windowInitInfo.height);

		m_windowSystem = std::make_shared<WindowSystem>();
		m_windowSystem->init(windowInitInfo);

		// gfx context will initialize glew which relies on a rendering context.
		// gfx context will also init ImGui Context.
		gfx::GfxContextInitParam gfxCtxParam{};
		gfxCtxParam.window = m_windowSystem;
		m_gfxContext = std::make_shared<gfx::GraphicContext>();
		m_gfxContext->init(gfxCtxParam);

		m_render = std::make_shared<render::RenderSystem>(m_gfxContext);

		m_pioWorld = std::make_shared<PioWorld>();
		// attach() must be called before init()
		m_pioWorld->attach(m_render->getScene());
		m_pioWorld->init();
	}

	void GlobalContext::swapData(uint64_t deltaMs)
	{
		m_pioWorld->swap(deltaMs);
		m_render->setWndSize(m_windowSystem->getWidth(), m_windowSystem->getHeight());
		m_render->dispatchEvent(m_eventMgr->processEvent());
	}

	void GlobalContext::shutdownSystems()
	{
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

		if (m_windowSystem)
		{
			m_windowSystem->shutdown();
			m_windowSystem.reset();
		}

		if (m_eventMgr)
		{
			m_eventMgr.reset();
		}

		LOG_DEBUG("all systems are shutdown");
		LogSystem::shutdown();
	}
}