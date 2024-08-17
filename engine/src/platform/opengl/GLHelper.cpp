#include <cstdio>
#include <stdarg.h>

#include "GLHelper.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "GLHelper"

namespace pio
{
	uint32_t GLHelper::BufferUsageToGLUsage(BufferUsage usage)
	{
		switch (usage)
		{
		case BufferUsage::Dynamic:
			return GL_DYNAMIC_DRAW;
		case BufferUsage::DynamicRead:
			return GL_DYNAMIC_READ;
		case BufferUsage::Static:
		default:
			return GL_STATIC_DRAW;
		}
	}

	bool GLHelper::CheckError(const char* fmt, ...)
	{
		GLenum ret = glGetError();
		if (ret == GL_NO_ERROR)
		{
			return true;
		}
		else
		{
			char buf[1024] = { 0 };
			va_list ap;
			va_start(ap, fmt);
			std::vsnprintf(buf, sizeof(buf) - 1, fmt, ap);
			va_end(ap);
			LOGE("%s: err code[0x%x]", buf, ret);
			return false;
		}
	}

	bool GLHelper::CheckFrameBufferStatus(const char* fmt, ...)
	{
		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		bool success = (status == GL_FRAMEBUFFER_COMPLETE);
		if (!success)
		{
			char buf[1024] = { 0 };
			va_list ap;
			va_start(ap, fmt);
			std::vsnprintf(buf, sizeof(buf) - 1, fmt, ap);
			va_end(ap);
			LOGE("%s: err code[0x%x]", buf, status);
			return false;
		}
		return success;
	}
}