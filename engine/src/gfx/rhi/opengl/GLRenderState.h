#ifndef __PIONNER_GFX_RHI_OPENGL_GL_RENDER_STATE_H__
#define __PIONNER_GFX_RHI_OPENGL_GL_RENDER_STATE_H__

#include "gfx/rhi/RenderState.h"

namespace pio
{
	class GLRenderState : public RenderState
	{
	public:
		GLRenderState(RenderBackendFlags flag);
		~GLRenderState() = default;

		virtual void setClear(const Clear& clear) override;
		virtual void setCullFace(const CullFace& cull) override;
		virtual void setBlendMode(const Blend& blend) override;
		virtual void setDepthTest(const DepthTest& depth) override;
		virtual void setStencilTest(const StencilTest& stencil) override;
	};
}

#endif