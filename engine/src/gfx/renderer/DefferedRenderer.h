#ifndef __PIONNER_GFX_RENDERER_DEFFERED_RENDERER_H__
#define __PIONNER_GFX_RENDERER_DEFFERED_RENDERER_H__

#include "Renderer.h"

namespace pio
{
	enum RenderBlockFlags;
	class RenderPass;
	class GBufferPass;
	class DefferedPass;
	class MainLightShadowCasterPass;

	class DefferedRenderer : public Renderer
	{
	public:
		DefferedRenderer();
		~DefferedRenderer() = default;

		virtual void onSetUp(PendingData& pendingData) override;
		virtual void onExecute(Ref<RenderContext> &context) override;

	protected:
		void initializeRenderingData(PendingData& pendingData);
		void executeBlock(RenderBlockFlags flag, std::vector<Ref<RenderPass>> &queue, Ref<RenderContext>& context);
		void executePass(Ref<RenderPass> &pass, Ref<RenderContext>& context);

	private:
		std::vector<Ref<RenderPass>> m_activeQueue;

		Ref<MainLightShadowCasterPass> m_mainLightShadowPass;
		Ref<DefferedPass> m_defferedPass;
		Ref<GBufferPass> m_GBufferPass;
	};
}

#endif