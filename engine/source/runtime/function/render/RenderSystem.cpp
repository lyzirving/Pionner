#include "function/render/RenderSystem.h"
#include "function/render/interface/opengl/RhiOpenGL.h"

#include "function/ui/WindowUI.h"

#include "core/log/LogSystem.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "RenderSystem"

namespace Pionner
{
	RenderSystem::RenderSystem() : m_rhi(nullptr)
	{
	}

	RenderSystem::~RenderSystem() = default;

	void RenderSystem::initialize(RenderSystemInitInfo& info)
	{
		OpenGLRhiInitInfo rhiInfo;
		rhiInfo.window = info.m_window;
		m_rhi = std::shared_ptr<Rhi>(new RhiOpenGL);
		m_rhi->initialize(&rhiInfo);
	}

	void RenderSystem::initializeUIRenderBackend(WindowUI* windowUI)
	{
	}

	void RenderSystem::shutdown()
	{
		if (m_rhi)
		{
			m_rhi->shutdown();
			m_rhi.reset();
		}
	}

	void RenderSystem::tick(float delta)
	{
	}
}