#ifndef __PIONNER_GFX_RENDERER_DEFFERED_RENDERER_H__
#define __PIONNER_GFX_RENDERER_DEFFERED_RENDERER_H__

#include "Renderer.h"

namespace pio
{
	enum RenderBlockFlags;
	class RenderPass;

	class DefferedRenderer : public Renderer
	{
	public:
		DefferedRenderer();
		~DefferedRenderer() = default;

		virtual void onAttach(Ref<RenderContext>& context) override;
		virtual void onDetach(Ref<RenderContext>& context) override;
		virtual void onSetUp() override;
		virtual void onExecute(Ref<RenderContext> &context) override;

	protected:
		void executeBlock(RenderBlockFlags flag, std::vector<Ref<RenderPass>>& queue, Ref<RenderContext>& context, Ref<RenderPass>& lastPass);

	private:
		std::vector<Ref<RenderPass>> m_passQueue;
		std::vector<Ref<RenderPass>> m_activeQueue;
	};
}

#endif