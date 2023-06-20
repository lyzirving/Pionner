#include "function/render/RenderSystem.h"
#include "function/render/WindowSystem.h"

#include "function/framework/world/World.h"
#include "function/framework/comp/LightComp.h"

#include "function/render/rhi/opengl/RhiGL.h"
#include "function/render/pipeline/RenderPipeline.h"
#include "function/render/shader/ShaderMgr.h"
#include "function/render/resource/RenderResourceMgr.h"

#include "function/render/scene/SceneMgr.h"

#include "function/ui/WindowUI.h"

#include "core/log/LogSystem.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "RenderSystem"

namespace Pionner
{
	RenderSystem::RenderSystem(const std::shared_ptr<World> &world)
		: BaseSystem(world)
		, m_rhi(nullptr)
		, m_pipeLine(nullptr)
		, m_sceneMgr(nullptr)
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

		m_pipeLine = std::make_shared<RenderPipeline>();
		m_pipeLine->m_rhi = m_rhi;
		RenderPipelineInitInfo pipelineInfo;
		pipelineInfo.window = info.window;
		m_pipeLine->initialize(pipelineInfo);

		m_sceneMgr = std::make_shared<SceneMgr>();
		SceneMgrInitInfo sceneMgrInfo;
		sceneMgrInfo.rhi = m_rhi;
		sceneMgrInfo.window = info.window;
		m_sceneMgr->initialize(sceneMgrInfo);

		m_shaderMgr = std::make_shared<ShaderMgr>();

		m_resourceMgr = std::make_shared<RenderResourceMgr>(m_rhi);
		RESOURCE_MGR_MAKE_SELF_WEAK(m_resourceMgr);
	}

	void RenderSystem::initializeUIRenderBackend(const std::shared_ptr<WindowUI> &ui)
	{
		if (m_pipeLine && ui)
		{
			m_pipeLine->initializeUIRenderBackend(ui);
		}
	}

	void RenderSystem::shutdownUIRenderBackend()
	{
		if (m_pipeLine)
		{
			m_pipeLine->shutdownUIRenderBackend();
		}
	}

	void RenderSystem::notifyRelease(BufferType type, uint32_t slot)
	{
		if (m_resourceMgr)
		{
			m_resourceMgr->notifyRelease(type, slot);
		}
	}

	void RenderSystem::swapData(float deltaTime)
	{
		RenderParam param{};
		param.sceneMgr = m_sceneMgr;
		param.rhi = m_rhi;
		param.resource = m_resourceMgr;
		param.shaderMgr = m_shaderMgr;
		param.world = m_world;

		auto &lights = m_world->getEntities(ENTITY_LIGHT);
		auto &lightComp = lights[0]->getComp<LightComp>();
		m_sceneMgr->swap(lightComp, param);
	}

	void RenderSystem::shutdown()
	{
		if (m_sceneMgr)
		{
			m_sceneMgr->shutdown();
			m_sceneMgr.reset();
		}

		if (m_shaderMgr)
		{
			m_shaderMgr->shutdown();
			m_shaderMgr.reset();
		}

		if (m_pipeLine)
		{
			m_pipeLine->shutdown();
			m_pipeLine.reset();
		}

		if (m_resourceMgr)
		{
			m_resourceMgr->shutdown();
			m_resourceMgr.reset();
		}

		if (m_rhi)
		{
			m_rhi->shutdown();
			m_rhi.reset();
		}
	}

	void RenderSystem::tick(float deltaTime)
	{
		RenderParam param{};
		param.sceneMgr = m_sceneMgr;
		param.rhi = m_rhi;
		param.resource = m_resourceMgr;
		param.shaderMgr = m_shaderMgr;
		param.world = m_world;

		m_resourceMgr->checkAbandoned();

		m_pipeLine->preparePassData(param);

		m_pipeLine->forwardRender(param);
	}
}