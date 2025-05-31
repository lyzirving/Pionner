#pragma once

#include "gfx/pass/RenderPass.h"

namespace pio
{
	class MainLitShadowCasterPass : public RenderPass
	{
		RTTR_ENABLE(RenderPass)
	public:
		MainLitShadowCasterPass(const std::string& name, RenderPassEvent event);
		~MainLitShadowCasterPass() = default;

		virtual void OnExecute(const Ref<RenderContext>& context, const Ref<CameraNode>& camera) override;
	};
}