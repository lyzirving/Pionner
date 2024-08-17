#ifndef __PIONNER_PLATFORM_GL_GLHELPER_H__
#define __PIONNER_PLATFORM_GL_GLHELPER_H__

#include "GLHeader.h"
#include "gfx/rhi/RhiDef.h"

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