#include "function/render/pipeline/RenderPipeline.h"
#include "function/render/scene/RenderScene.h"
#include "function/render/interface/Rhi.h"
#include "function/render/pass/UIPass.h"

namespace Pionner
{
	RenderPipeline::RenderPipeline() : RenderPipelineBase()
	{
	}

	RenderPipeline::~RenderPipeline() = default;

	void RenderPipeline::initialize(RenderPipelineInitInfo &info)
	{
		RenderPassCommonInfo passCommon;
		passCommon.rhi = m_rhi;

		m_uiPass = std::shared_ptr<RenderPassBase>(new UIPass);
		m_uiPass->setPassCommonInfo(passCommon);

		RenderPassInitInfo passInitInfo;
		m_uiPass->initialize(passInitInfo);
	}

	void RenderPipeline::shutdownUIRenderBackend()
	{
		m_uiPass->shutdownUIRenderBackend();
	}

	void RenderPipeline::shutdown()
	{
		m_uiPass->shutdown();
	}

	void RenderPipeline::forwardRender(const std::shared_ptr<RenderScene> &scene)
	{
		m_rhi->viewportFull();

		if (scene)
			scene->forwardRender();

		m_uiPass->draw();
	}
}