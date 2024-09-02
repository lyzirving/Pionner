#ifndef __PIONNER_GFX_RHI_RENDER_STATE_H__
#define __PIONNER_GFX_RHI_RENDER_STATE_H__

#include "RenderStateAttrs.h"

namespace pio
{
	class RenderState
	{
	public:
		RenderState(RenderBackendFlags flag) : m_renderBackend(flag) {}
		virtual ~RenderState() = default;

		virtual void setClear(const Clear& clear) = 0;
		virtual void setCullFace(const CullFace& cull) = 0;
		virtual void setBlendMode(const Blend& blend) = 0;
		virtual void setDepthTest(const DepthTest& depth) = 0;
		virtual void setStencilTest(const StencilTest& stencil) = 0;
		virtual void setStateMachine(const RenderStateAttrs& attrs) = 0;

		RenderBackendFlags renderBackend() const { return m_renderBackend; }

	protected:
		const RenderBackendFlags m_renderBackend;

	public:
		static Ref<RenderState> Create(RenderBackendFlags flag);
	};
}

#endif