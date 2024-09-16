#ifndef __PIONNER_GFX_PASS_GBUFFER_PASS_H__
#define __PIONNER_GFX_PASS_GBUFFER_PASS_H__

#include "RenderPass.h"

namespace pio
{
	class GBufferPass : public RenderPass
	{
		OVERRIDE_PASS_TYPE(RenderPassType::GBuffer)
	public:
		GBufferPass(const std::string &name, RenderPassEvent event) : RenderPass(name, event) {}
		~GBufferPass() = default;

		virtual void onAttach(Ref<RenderContext>& context) override;
		virtual void onDetach(Ref<RenderContext>& context) override;
		virtual void onExecute(Ref<RenderContext>& context, Ref<CameraNode>& camera, Ref<RenderPass>& lastPass) override;
	};

	template<>
	bool RenderPass::is<GBufferPass>() const;
}

#endif