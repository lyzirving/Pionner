#ifndef __PIONNER_GFX_PASS_MAIN_LIGHT_SHADOW_CASTER_PASS_H__
#define __PIONNER_GFX_PASS_MAIN_LIGHT_SHADOW_CASTER_PASS_H__

#include "RenderPass.h"

namespace pio
{
	class MainLightShadowCasterPass : public RenderPass
	{
		OVERRIDE_PASS_TYPE(RenderPassType::MainLightShadowCaster)
	public:
		MainLightShadowCasterPass(const std::string &name, RenderPassEvent event) : RenderPass(name, event) {}
		~MainLightShadowCasterPass() = default;

		virtual void onExecute(Ref<RenderContext>& context, Ref<CameraNode>& camNode, Ref<RenderPass>& lastPass) override;
	};

	template<>
	bool RenderPass::is<MainLightShadowCasterPass>() const;
}

#endif