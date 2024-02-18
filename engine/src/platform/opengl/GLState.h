#ifndef __PIONNER_PLATFORM_OPENGL_GL_STATE_H__
#define __PIONNER_PLATFORM_OPENGL_GL_STATE_H__

#include "gfx/renderer/RenderState.h"

namespace pio
{
	class GLState
	{
	public:
		static void SetBlendMode(const Blend &blend);
		static void SetClear(const Clear &clear);
		static void SetCullFace(const CullFace &cull);
		static void SetDepthTest(const DepthTest &depth);
		static void SetStencilTest(const StencilTest &stencil);

	private:
		GLState() {}
		~GLState() = default;
	};
}

#endif