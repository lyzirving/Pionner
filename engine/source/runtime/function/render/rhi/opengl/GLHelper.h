#ifndef __GL_HELPER_H__
#define __GL_HELPER_H__

#include <cstdint>
#include <string>

namespace Pionner
{
	class GLHelper
	{
	public:
		static uint32_t buildProgram(const char *vert, const char *frag);
		static uint32_t buildProgram(const char *vert, const char *frag, const char *geo);
		static bool     checkGLErr(const char *fmt, ...);
		static uint32_t colorFormat(int32_t component);

	private:
		static bool createShader(uint32_t type, const char *source, uint32_t &shader);
		static void logShaderInfo(uint32_t shader);
		static void logLinkStatus(uint32_t program);

		GLHelper() {}
		~GLHelper() {}
	};
}

#endif