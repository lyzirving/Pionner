#pragma once

#include "gfx/pass/RenderPass.h"

namespace pio
{
	class DefferedPass : public RenderPass
	{
		RTTR_ENABLE(RenderPass)
	public:
		DefferedPass(const std::string &name, RenderPassEvent event) : RenderPass(name, event) {}
		~DefferedPass() = default;

		virtual void OnAttach(const Ref<RenderContext>& context) override;
		virtual void OnDetach(const Ref<RenderContext>& context) override;
		virtual void OnExecute(const Ref<RenderContext>& context, const Ref<CameraNode>& camera) override;
	};
}