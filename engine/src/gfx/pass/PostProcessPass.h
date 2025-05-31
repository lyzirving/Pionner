#pragma once

#include "gfx/pass/RenderPass.h"

namespace pio
{
	class UIPass;

	class PostProcessPass : public RenderPass
	{
		RTTR_ENABLE(RenderPass)
	public:
		PostProcessPass(const std::string& name, RenderPassEvent event) : RenderPass(name, event) {}
		~PostProcessPass() = default;

		virtual void OnAttach(const Ref<RenderContext>& context) override;		
		virtual void OnExecute(const Ref<RenderContext>& context, const Ref<CameraNode>& camera) override;

	private:
		Ref<UIPass> m_UIPass;
	};
}