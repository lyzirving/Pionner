#pragma once

#include "gfx/pass/RenderPass.h"

namespace pio
{
	class ForwardPass : public RenderPass
	{
		RTTR_ENABLE(RenderPass)
	public:
		ForwardPass(const std::string &name, RenderPassEvent event) : RenderPass(name, event) {}
		~ForwardPass() = default;

		virtual void OnAttach(const Ref<RenderContext>& context) override;
		virtual void OnDetach(const Ref<RenderContext>& context) override;
		virtual void OnExecute(const Ref<RenderContext>& context, const Ref<CameraNode>& camera) override;
	};
}