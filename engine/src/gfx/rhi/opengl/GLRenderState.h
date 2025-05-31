#pragma once

#include "gfx/rhi/RenderState.h"

namespace pio
{
	class GLRenderState : public RenderState
	{
		RTTR_ENABLE(RenderState)
	public:
		GLRenderState(RenderBackendFlags flag);
		~GLRenderState() = default;

		virtual void SetClear(const Clear& clear) override;
		virtual void SetCullFace(const CullFace& cull) override;
		virtual void SetBlendMode(const Blend& blend) override;
		virtual void SetDepthTest(const DepthTest& depth) override;
		virtual void SetStencilTest(const StencilTest& stencil) override;

		virtual void ApplyStateChange(const RenderStateAttrs& attrs) override;
		virtual void SetStateMachine(const RenderStateAttrs &attrs) override;
	};
}