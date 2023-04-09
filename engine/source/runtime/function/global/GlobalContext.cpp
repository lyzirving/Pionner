#include "function/global/GlobalContext.h"
#include "function/render/WindowSystem.h"
#include "function/render/RenderSystem.h"
#include "function/framework/assets/AssetsSystem.h"

#include "core/log/LogSystem.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "GlobalContext"

namespace Pionner
{
	GlobalContext g_runtimeCtx{};

	GlobalContext::GlobalContext() : m_windowSystem(nullptr)
		                           , m_renderSystem(nullptr) 
		                           , m_assetsSystem(nullptr)
	{
	}

	GlobalContext::~GlobalContext() = default;

	void GlobalContext::startSystems()
	{
		LogSystem::initialize();

		WindowCreateInfo windowInitInfo;
		m_windowSystem = std::make_shared<WindowSystem>();
		m_windowSystem->initialize(windowInitInfo);

		RenderSystemInitInfo renderInitInfo;
		renderInitInfo.window = m_windowSystem;
		m_renderSystem = std::make_shared<RenderSystem>();
		m_renderSystem->initialize(renderInitInfo);

		m_assetsSystem = std::make_shared<AssetsSystem>();
		m_assetsSystem->initialize();
	}

	void GlobalContext::shutdownSystems()
	{
		if (m_assetsSystem)
		{
			m_assetsSystem->shutdown();
			m_assetsSystem.reset();
		}

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

		LOG_DEBUG("all systems are shutdown");
		LogSystem::shutdown();
	}
}