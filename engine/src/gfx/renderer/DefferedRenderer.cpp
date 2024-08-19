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

	void DefferedRenderer::onSetUp()
	{
		m_passList.clear();
		m_passList.push_back(m_mainLightShadowPass);
		m_passList.push_back(m_GBufferPass);
		m_passList.push_back(m_defferedPass);

		std::sort(m_passList.begin(), m_passList.end(), RenderPass::PassSorter);
	}

	void DefferedRenderer::onRender(Ref<RenderContext> &context)
	{
		for(size_t i = 0; i < m_passList.size(); i++)
		{
			
		}
	}
}