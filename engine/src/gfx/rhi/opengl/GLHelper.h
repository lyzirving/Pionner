#ifndef __PIONNER_GFX_RHI_OPENGL_GL_HELPER_H__
#define __PIONNER_GFX_RHI_OPENGL_GL_HELPER_H__

#include "GLHeader.h"
#include "gfx/GfxDef.h"

namespace pio
{
	class GLHelper
	{
	public:
		static uint32_t BufferUsageToGLUsage(BufferUsage usage);
		static bool CheckError(const char *fmt, ...);
		static bool CheckFrameBufferStatus(const char *fmt, ...);
	};
}

#endif