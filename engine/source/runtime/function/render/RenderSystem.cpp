#include "function/render/RenderSystem.h"
#include "function/render/interface/opengl/RhiOpenGL.h"
#include "function/render/pipeline/RenderPipeline.h"
#include "function/render/scene/RenderScene.h"
#include "function/render/swap/SwapContext.h"
#include "function/render/WindowSystem.h"

#include "function/ui/WindowUI.h"

#include "core/log/LogSystem.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "RenderSystem"

namespace Pionner
{
	RenderSystem::RenderSystem() : m_rhi(nullptr)
		, m_pipeLine(nullptr)
		, m_scene(nullptr)
	{
	}

	RenderSystem::~RenderSystem() = default;

	void RenderSystem::initialize(RenderSystemInitInfo &info)
	{
		OpenGLRhiInitInfo rhiInfo;
		rhiInfo.window = info.window;
		m_rhi = std::shared_ptr<Rhi>(new RhiOpenGL);
		m_rhi->initialize(&rhiInfo);

		m_pipeLine = std::shared_ptr<RenderPipelineBase>(new RenderPipeline);
		m_pipeLine->m_rhi = m_rhi;
		RenderPipelineInitInfo pipelineInfo;
		m_pipeLine->initialize(pipelineInfo);

		m_scene = std::make_shared<RenderScene>();
		m_scene->initialize(m_rhi);
	}

	void RenderSystem::initializeUIRenderBackend(WindowUI *windowUI)
	{
		m_pipeLine->initializeUIRenderBackend(windowUI);
	}

	void RenderSystem::shutdownUIRenderBackend()
	{
		m_pipeLine->shutdownUIRenderBackend();
	}

	void RenderSystem::shutdown()
	{
		if (m_scene)
		{
			m_scene->shutdown();
			m_scene.reset();
		}

		if (m_pipeLine)
		{
			m_pipeLine->shutdown();
			m_pipeLine.reset();
		}

		if (m_rhi)
		{
			m_rhi->shutdown();
			m_rhi.reset();
		}
	}

	void RenderSystem::processSwapData(const SwapData &data)
	{
		if (!data.m_entities.empty())
		{
			m_scene->addEntities(data.m_entities);
		}
	}

	void RenderSystem::tick(float delta)
	{
		m_pipeLine->preparePassData();

		m_pipeLine->forwardRender(m_scene);
	}
}