#pragma once

#include "gfx/pass/RenderPass.h"

namespace pio
{
	class PointLitShadowCasterPass : public RenderPass
	{
		RTTR_ENABLE(RenderPass)
	public:
		PointLitShadowCasterPass(const std::string& name, RenderPassEvent event);
		~PointLitShadowCasterPass() = default;

		virtual void OnExecute(const Ref<RenderContext>& context, const Ref<CameraNode>& camera) override;
	};
}