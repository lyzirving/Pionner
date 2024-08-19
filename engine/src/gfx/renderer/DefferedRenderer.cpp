#include "DefferedRenderer.h"

#include "gfx/pass/MainLightShadowCasterPass.h"
#include "gfx/pass/DefferedPass.h"
#include "gfx/pass/GBufferPass.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "DefferedRenderer"

namespace pio
{
	DefferedRenderer::DefferedRenderer() : Renderer()
	{
		LOGD("Deffered Renderer create");
		m_mainLightShadowPass = CreateRef<MainLightShadowCasterPass>("MainLightShadowCasterPass", RenderPassEvent::RenderingShadows);		
		m_GBufferPass = CreateRef<GBufferPass>("GBufferPass", RenderPassEvent::RenderingOpaques);
		m_defferedPass = CreateRef<DefferedPass>("DefferedPass", RenderPassEvent::AfterRenderingOpaques);
	}

	void DefferedRenderer::onSetUp(PendingData& pendingData)
	{
		initializeRenderingData(pendingData);

		m_activeQueue.clear();
		m_activeQueue.push_back(m_mainLightShadowPass);
		m_activeQueue.push_back(m_GBufferPass);
		m_activeQueue.push_back(m_defferedPass);

		if (m_activeQueue.size() > 1)
			std::sort(m_activeQueue.begin(), m_activeQueue.end(), RenderPass::PassSorter);		
	}

	void DefferedRenderer::onExecute(Ref<RenderContext> &context)
	{
		executeBlock(RenderBlockFlags::MainBeforeRendering, m_activeQueue, context);
		executeBlock(RenderBlockFlags::MainRenderingOpaque, m_activeQueue, context);
		executeBlock(RenderBlockFlags::MainRenderingTransparents, m_activeQueue, context);
		executeBlock(RenderBlockFlags::MainAfterRendering, m_activeQueue, context);
	}

	void DefferedRenderer::initializeRenderingData(PendingData& pendingData)
	{
	}

	void DefferedRenderer::executeBlock(RenderBlockFlags flag, std::vector<Ref<RenderPass>>& queue, Ref<RenderContext>& context)
	{
		BlockRange range = RenderBlock::GetBlockRange(flag);
		for (size_t i = 0; i < queue.size(); i++)
		{
			if (range.contains(queue[i]))
			{
				executePass(queue[i], context);
			}
		}
	}

	void DefferedRenderer::executePass(Ref<RenderPass>& pass, Ref<RenderContext>& context)
	{		
	}
}