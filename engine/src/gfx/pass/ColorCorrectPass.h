#ifndef __PIONNER_GFX_PASS_COLOR_CORRECT_PASS_H__
#define __PIONNER_GFX_PASS_COLOR_CORRECT_PASS_H__

#include "RenderPass.h"

namespace pio
{
	class ColorCorrectPass : public RenderPass
	{
		OVERRIDE_PASS_TYPE(RenderPassType::ColorCorrect)
	public:
		ColorCorrectPass(const std::string& name, RenderPassEvent event) : RenderPass(name, event) {}
		~ColorCorrectPass() = default;

		virtual void onAttach(Ref<RenderContext>& context) override;
		virtual void onExecute(Ref<RenderContext>& context, Ref<CameraNode>& camNode, Ref<RenderPass>& lastPass) override;
	};

	template<>
	bool RenderPass::is<ColorCorrectPass>() const;
}

#endif