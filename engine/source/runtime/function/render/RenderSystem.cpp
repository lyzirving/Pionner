#include "function/render/RenderSystem.h"
#include "function/render/interface/opengl/RhiOpenGL.h"
#include "function/render/pipeline/RenderPipeline.h"
#include "function/render/scene/RenderScene.h"
#include "function/render/scene/Camera.h"
#include "function/render/scene/Frustum.h"
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
		, m_camera(nullptr)
		, m_frustum(nullptr)
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

		m_camera = std::make_shared<Camera>();
		m_camera->setPosition(glm::vec3(0.f, 3.f, 5.f));

		m_frustum = std::make_shared<Frustum>();
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
		m_frustum.reset();
		m_camera.reset();

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
		if (!data.m_objEntities.empty())
		{
			m_scene->addEntities(data.m_objEntities);
		}
	}

	void RenderSystem::tick(float delta)
	{
		m_pipeLine->preparePassData();

		m_pipeLine->forwardRender(m_scene, m_camera, m_frustum);
	}
}