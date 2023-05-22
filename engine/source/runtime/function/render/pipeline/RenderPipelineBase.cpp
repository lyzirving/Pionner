#include "function/render/pipeline/RenderPipelineBase.h"
#include "function/render/rhi/Rhi.h"
#include "function/render/pass/RenderPassBase.h"

#include "function/ui/WindowUI.h"

namespace Pionner
{
	RenderPipelineBase::RenderPipelineBase()
		: m_rhi(nullptr)
		, m_uiPass(nullptr)
	{
	}

	RenderPipelineBase::~RenderPipelineBase()
	{
		m_uiPass.reset();
		m_rhi.reset();
	}

	void RenderPipelineBase::forwardRender(RenderParam &param)
	{
	}

	void RenderPipelineBase::initializeUIRenderBackend(WindowUI *ui)
	{
		if (m_uiPass) m_uiPass->initializeUIRenderBackend(ui);
	}

	void RenderPipelineBase::shutdownUIRenderBackend()
	{
		if (m_uiPass) m_uiPass->shutdownUIRenderBackend();
	}

	void RenderPipelineBase::preparePassData()
	{
	}
}