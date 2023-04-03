#include "function/global/GlobalContext.h"
#include "function/render/WindowSystem.h"
#include "function/render/interface/opengl/RhiOpenGL.h"

#include "core/log/LogSystem.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "GlobalContext"

namespace Pionner
{
	GlobalContext g_runtimeCtx{};

	GlobalContext::GlobalContext() {}

	GlobalContext::~GlobalContext() = default;

	void GlobalContext::startSystems()
	{
		LogSystem::initialize();

		WindowCreateInfo info;
		m_windowSystem = std::make_shared<WindowSystem>();
		m_windowSystem->initialize(info);

		OpenGLRhiInitInfo rhiInfo;
		rhiInfo.window = m_windowSystem;
		m_rhi = std::shared_ptr<Rhi>(new RhiOpenGL);
		m_rhi->initialize(&rhiInfo);
	}

	void GlobalContext::shutdownSystems()
	{
		LOG_DEBUG("shutdown all systems");
		LogSystem::shutdown();

		if (m_rhi)
		{
			m_rhi->shutdown();
			m_rhi.reset();
		}

		if (m_windowSystem)
		{
			m_windowSystem->shutdown();
			m_windowSystem.reset();
		}
	}
}