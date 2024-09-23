#ifndef __PIONNER_GFX_PASS_UI_PASS_H__
#define __PIONNER_GFX_PASS_UI_PASS_H__

#include "RenderPass.h"

namespace pio
{
	class UiPass : public RenderPass
	{
		OVERRIDE_PASS_TYPE(RenderPassType::UI)
	public:
		UiPass(const std::string& name, RenderPassEvent event) : RenderPass(name, event) {}
		~UiPass() = default;

		virtual void onAttach(Ref<RenderContext>& context) override;
		virtual void onExecute(Ref<RenderContext>& context, Ref<CameraNode>& camNode, Ref<RenderPass>& lastPass) override;
	};

	template<>
	bool RenderPass::is<UiPass>() const;
}

#endif