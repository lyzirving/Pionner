#include "DefferedRenderer.h"

#include "gfx/pass/MainLightShadowCasterPass.h"
#include "gfx/pass/DefferedPass.h"
#include "gfx/pass/GBufferPass.h"

#include "gfx/renderer/RenderContext.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "DefferedRenderer"

namespace pio
{
	DefferedRenderer::DefferedRenderer() : Renderer()
	{
		LOGD("Deffered Renderer create");
		m_passQueue.push_back(CreateRef<MainLightShadowCasterPass>("MainLightShadowCasterPass", RenderPassEvent::RenderingShadows));
		m_passQueue.push_back(CreateRef<GBufferPass>("GBufferPass", RenderPassEvent::RenderingOpaques));
		m_passQueue.push_back(CreateRef<DefferedPass>("DefferedPass", RenderPassEvent::AfterRenderingOpaques));
	}

	void DefferedRenderer::onAttach(Ref<RenderContext>& context)
	{
		for (auto &pass : m_passQueue)
		{
			pass->onAttach(context);
		}
	}

	void DefferedRenderer::onDetach(Ref<RenderContext>& context)
	{
		for (auto& pass : m_passQueue)
		{
			pass->onDetach(context);
		}
	}

	void DefferedRenderer::onSetUp()
	{
		m_activeQueue.clear();
		for (auto& pass : m_passQueue)
		{
			if (pass->bActive())
				m_activeQueue.push_back(pass);			
		}
		if (m_activeQueue.size() > 1)
			std::sort(m_activeQueue.begin(), m_activeQueue.end(), RenderPass::PassSorter);		
	}

	void DefferedRenderer::onExecute(Ref<RenderContext> &context, Ref<CameraNode>& camNode)
	{
		Ref<RenderPass> lastPass;
		executeBlock(RenderBlockFlags::MainBeforeRendering, m_activeQueue, context, camNode, lastPass);
		executeBlock(RenderBlockFlags::MainRenderingOpaque, m_activeQueue, context, camNode, lastPass);
		executeBlock(RenderBlockFlags::MainRenderingTransparents, m_activeQueue, context, camNode, lastPass);
		executeBlock(RenderBlockFlags::MainAfterRendering, m_activeQueue, context, camNode, lastPass);
	}

	void DefferedRenderer::executeBlock(RenderBlockFlags flag, std::vector<Ref<RenderPass>>& queue, Ref<RenderContext>& context, Ref<CameraNode>& camNode, Ref<RenderPass>& lastPass)
	{		
		BlockRange range = RenderBlock::GetBlockRange(flag);
		if(range.intersect(queue))
		{
			for(size_t i = 0; i < queue.size(); i++)
			{
				if(range.contains(queue[i]))
				{					
					queue[i]->onExecute(context, camNode, lastPass);
					lastPass = queue[i];
				}
			}
		}
	}
}