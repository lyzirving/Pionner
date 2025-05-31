#pragma once

#include "gfx/pass/RenderPass.h"

namespace pio
{
	class UIPass : public RenderPass
	{
		RTTR_ENABLE(RenderPass)
	public:
		UIPass(const std::string& name, RenderPassEvent event) : RenderPass(name, event) {}
		~UIPass() = default;

		virtual void OnAttach(const Ref<RenderContext>& context) override;
		virtual void OnExecute(const Ref<RenderContext>& context, const Ref<CameraNode>& camera) override;
	};
}