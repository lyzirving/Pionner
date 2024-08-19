#ifndef __PIONNER_GFX_RENDERER_DEFFERED_RENDERER_H__
#define __PIONNER_GFX_RENDERER_DEFFERED_RENDERER_H__

#include "Renderer.h"

namespace pio
{
	class RenderPass;
	class GBufferPass;
	class DefferedPass;
	class MainLightShadowCasterPass;

	class DefferedRenderer : public Renderer
	{
	public:
		DefferedRenderer();
		~DefferedRenderer() = default;

		virtual void onSetUp() override;
		virtual void onRender(Ref<RenderContext> &context) override;

	private:
		std::vector<Ref<RenderPass>> m_passList;

		Ref<MainLightShadowCasterPass> m_mainLightShadowPass;
		Ref<DefferedPass> m_defferedPass;
		Ref<GBufferPass> m_GBufferPass;
	};
}

#endif