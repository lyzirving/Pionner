#include "DefferedRenderer.h"

#include "gfx/pass/MainLitShadowCasterPass.h"
#include "gfx/pass/PointLitShadowCasterPass.h"
#include "gfx/pass/GBufferPass.h"
#include "gfx/pass/DefferedPass.h"
#include "gfx/pass/PostProcessPass.h"

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
		auto mainLitShadowCasterPass = CreateRef<MainLitShadowCasterPass>("MainLitShadowCasterPass", RenderingShadows);
		auto pointLitShadowCasterPass = CreateRef<PointLitShadowCasterPass>("PointLitShadowCasterPass", RenderPassEvent(RenderingShadows + 1));
		auto gPass = CreateRef<GBufferPass>("GBufferPass", RenderingOpaques);		
		auto deferredPass = CreateRef<DefferedPass>("DefferedPass", AfterRenderingOpaques);
		auto postProcess = CreateRef<PostProcessPass>("PostProcessPass", RenderingPostProcessing);

		m_PassMap.insert({ mainLitShadowCasterPass->GetClassName(), mainLitShadowCasterPass });
		m_PassMap.insert({ pointLitShadowCasterPass->GetClassName(), pointLitShadowCasterPass });
		m_PassMap.insert({ gPass->GetClassName(), gPass});
		m_PassMap.insert({ deferredPass->GetClassName(), deferredPass});
		m_PassMap.insert({ postProcess->GetClassName(), postProcess });
	}

	void DefferedRenderer::OnAttach(const Ref<RenderContext>& context)
	{
		for (auto it : m_PassMap)
			it.second->OnAttach(context);
	}

	void DefferedRenderer::OnDetach(const Ref<RenderContext>& context)
	{
		for (auto it : m_PassMap)
			it.second->OnDetach(context);
	}

	void DefferedRenderer::OnSetUp()
	{
		m_ActiveQueue.clear();
		for (auto it : m_PassMap)
			if(it.second->bActive())
				m_ActiveQueue.push_back(it.second);

		if (m_ActiveQueue.size() > 1)
			std::sort(m_ActiveQueue.begin(), m_ActiveQueue.end(), RenderPass::PassSorter);		
	}

	void DefferedRenderer::OnExecute(const Ref<RenderContext>& context, const Ref<CameraNode>& camNode)
	{
		OnSetUp();
		ExecuteBlock(RenderBlockFlags::MainBeforeRendering, m_ActiveQueue, context, camNode);
		ExecuteBlock(RenderBlockFlags::MainRenderingOpaque, m_ActiveQueue, context, camNode);
		ExecuteBlock(RenderBlockFlags::MainRenderingTransparents, m_ActiveQueue, context, camNode);
		ExecuteBlock(RenderBlockFlags::MainAfterRendering, m_ActiveQueue, context, camNode);
	}

	void DefferedRenderer::ExecuteBlock(RenderBlockFlags flag, const std::vector<Ref<RenderPass>>& queue, const Ref<RenderContext>& context, const Ref<CameraNode>& camNode)
	{
		BlockRange range = RenderBlock::GetBlockRange(flag);
		if(range.Intersect(queue))
		{
			for(size_t i = 0; i < queue.size(); i++)
			{
				if(range.Contains(queue[i]))
				{
					queue[i]->OnExecute(context, camNode);
				}
			}
		}
	}
}