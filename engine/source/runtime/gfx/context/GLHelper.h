#ifndef __PIONNER_GFX_CONTEXT_GL_HELPER_H__
#define __PIONNER_GFX_CONTEXT_GL_HELPER_H__

#include <cstdint>
#include <string>

#include "gfx/buffer/Texture.h"

namespace pio
{
	namespace gfx
	{
		enum DataType
		{
			DATA_UNSIGNED_BYTE,
			DATA_UNSIGNED_SHORT,
			DATA_UNSIGNED_INT
		};

		class GLHelper
		{
		public:
			static uint32_t buildProgram(const char *vert, const char *frag);
			static uint32_t buildProgram(const char *vert, const char *frag, const char *geo);
			static bool     checkGLErr(const char *fmt, ...);
			static uint32_t colorFormat(int32_t component);

			static bool drawTriangleElements(uint32_t indexCnt, DataType dataType, const void *indices = nullptr);
			static void unbindTexture(TextureType type);

		private:
			static bool createShader(uint32_t type, const char *source, uint32_t &shader);
			static void logShaderInfo(uint32_t shader);
			static void logLinkStatus(uint32_t program);

			static uint32_t getDataType(DataType type);

			GLHelper() {}
			~GLHelper() {}
		};
	}
}

#endif