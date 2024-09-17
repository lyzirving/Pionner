#ifndef __PIONNER_GFX_PASS_DEFFERED_PASS_H__
#define __PIONNER_GFX_PASS_DEFFERED_PASS_H__

#include "RenderPass.h"

namespace pio
{
	class DefferedPass : public RenderPass
	{
		OVERRIDE_PASS_TYPE(RenderPassType::Deferred)
	public:
		DefferedPass(const std::string &name, RenderPassEvent event) : RenderPass(name, event) {}
		~DefferedPass() = default;

		virtual void onAttach(Ref<RenderContext>& context) override;
		virtual void onDetach(Ref<RenderContext>& context) override;
		virtual void onExecute(Ref<RenderContext> &context, Ref<CameraNode>& camNode, Ref<RenderPass>& lastPass) override;
	};

	template<>
	bool RenderPass::is<DefferedPass>() const;
}

#endif