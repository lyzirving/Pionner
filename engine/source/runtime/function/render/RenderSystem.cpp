#include "function/render/RenderSystem.h"
#include "function/render/WindowSystem.h"

#include "function/render/rhi/opengl/RhiGL.h"
#include "function/render/pipeline/RenderPipeline.h"
#include "function/render/swap/SwapContext.h"
#include "function/render/shader/ShaderMgr.h"
#include "function/render/resource/RenderResourceMgr.h"

#include "function/render/scene/RenderScene.h"
#include "function/render/scene/Camera.h"
#include "function/render/scene/Frustum.h"

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
		, m_shaderMgr(nullptr)
		, m_resourceMgr(nullptr)
	{
	}

	RenderSystem::~RenderSystem() = default;

	void RenderSystem::initialize(RenderSystemInitInfo &info)
	{
		OpenGLRhiInitInfo rhiInfo;
		rhiInfo.window = info.window;
		m_rhi = std::shared_ptr<Rhi>(new RhiGL);
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
		m_frustum->setAspect(float(info.window->getWidth()) / float(info.window->getHeight()));

		m_shaderMgr = std::make_shared<ShaderMgr>();

		m_resourceMgr = std::make_shared<RenderResourceMgr>(m_rhi);
		RESOURCE_MGR_MAKE_SELF_WEAK(m_resourceMgr);
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
		m_resourceMgr.reset();

		m_frustum.reset();

		m_camera.reset();

		if (m_scene)
		{
			m_scene->shutdown();
			m_scene.reset();
		}

		if (m_shaderMgr)
		{
			m_shaderMgr->destroy();
			m_shaderMgr.reset();
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

		RenderParam param{ m_camera, m_frustum, m_resourceMgr, m_shaderMgr, m_rhi };

		m_pipeLine->forwardRender(m_scene, param);
	}
}