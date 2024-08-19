#ifndef __PIONNER_GFX_PASS_MAIN_LIGHT_SHADOW_CASTER_PASS_H__
#define __PIONNER_GFX_PASS_MAIN_LIGHT_SHADOW_CASTER_PASS_H__

#include "RenderPass.h"

namespace pio
{
	class MainLightShadowCasterPass : public RenderPass
	{
	public:
		MainLightShadowCasterPass(const std::string &name, RenderPassEvent event) : RenderPass(name, event) {}
		~MainLightShadowCasterPass() = default;
	};
}

#endif