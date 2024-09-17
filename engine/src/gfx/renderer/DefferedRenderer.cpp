#include "DefferedRenderer.h"

#include "gfx/pass/MainLightShadowCasterPass.h"
#include "gfx/pass/DefferedPass.h"
#include "gfx/pass/GBufferPass.h"
#include "gfx/pass/ColorCorrectPass.h"

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
		auto mainShadowCasterPass = CreateRef<MainLightShadowCasterPass>("MainLightShadowCasterPass", RenderPassEvent::RenderingShadows);
		auto GPass = CreateRef<GBufferPass>("GBufferPass", RenderPassEvent::RenderingOpaques);
		auto deferredPass = CreateRef<DefferedPass>("DefferedPass", RenderPassEvent::AfterRenderingOpaques);
		auto colorCorrectPass = CreateRef<ColorCorrectPass>("ColorCorrectPass", RenderPassEvent::BeforeRenderingPostProcessing);

		m_passMap.insert({ (uint32_t)mainShadowCasterPass->type(), mainShadowCasterPass });
		m_passMap.insert({ (uint32_t)GPass->type(), GPass });
		m_passMap.insert({ (uint32_t)deferredPass->type(), deferredPass }); 
		m_passMap.insert({ (uint32_t)colorCorrectPass->type(), colorCorrectPass });
	}

	void DefferedRenderer::onAttach(Ref<RenderContext>& context)
	{
		for (auto it : m_passMap)
		{
			it.second->onAttach(context);
		}
	}

	void DefferedRenderer::onDetach(Ref<RenderContext>& context)
	{
		for (auto it : m_passMap)
		{
			it.second->onDetach(context);
		}
	}

	void DefferedRenderer::onSetUp()
	{
		m_activeQueue.clear();
		for (auto it : m_passMap)
		{
			if (it.second->bActive())
			{
				m_activeQueue.push_back(it.second);
			}
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

	Ref<RenderPass> DefferedRenderer::getPass(uint32_t type)
	{
		auto it = m_passMap.find(type);
		if (it != m_passMap.end())
		{
			return it->second;
		}
		return Ref<RenderPass>();
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