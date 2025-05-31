#pragma once

#include "RenderStateAttrs.h"

namespace pio
{
	class RenderState : public Object
	{
		RTTR_ENABLE(Object)
	public:
		RenderState(RenderBackendFlags flag) : m_RenderBackend(flag) {}
		virtual ~RenderState() = default;

		virtual void SetClear(const Clear& clear) = 0;
		virtual void SetCullFace(const CullFace& cull) = 0;
		virtual void SetBlendMode(const Blend& blend) = 0;
		virtual void SetDepthTest(const DepthTest& depth) = 0;
		virtual void SetStencilTest(const StencilTest& stencil) = 0;

		virtual void ApplyStateChange(const RenderStateAttrs& attrs) = 0;
		virtual void SetStateMachine(const RenderStateAttrs& attrs) = 0;

		RenderBackendFlags RenderBackend() const { return m_RenderBackend; }

	protected:
		const RenderBackendFlags m_RenderBackend;

		Clear m_Clear;
		CullFace m_Cull;
		Blend m_Blend;
		DepthTest m_DepthTest;
		StencilTest m_Stencil;

	public:
		static Ref<RenderState> Create(RenderBackendFlags flag);
	};
}