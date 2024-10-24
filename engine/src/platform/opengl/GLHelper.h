#ifndef __PIONNER_PLATFORM_GL_GLHELPER_H__
#define __PIONNER_PLATFORM_GL_GLHELPER_H__

#include "GLHeader.h"

#include "gfx/rhi/VertexBuffer.h"
#include "gfx/struct/Image.h"

namespace pio
{
	class GLHelper
	{
	public:
		static uint32_t BufferUsageToGLUsage(BufferUsage usage);
		static bool CheckError(const char *fmt, ...);
		static bool CheckFrameBufferStatus(const char *fmt, ...);
		/*
		* @brief: Create OpenGL shader program.
		*         If stageNum is 2, stage source is vertex, fragment.
		*         If stageNum is 3, stage source is vertex, fragment, geometry.
		*         If compie stage contains tessellation or computation, reference to other specific APIs.
		*/
		static uint32_t CreateSimpleProgram(const uint32_t stageNum, const std::initializer_list<std::string> &stageSource);
		static uint32_t CreateShaderProgram(const char *vert, const char *frag);
		static uint32_t CreateShaderProgram(const char *vert, const char *frag, const char *geo);
		static uint32_t ShaderDataTypeToGLBaseType(ShaderDataType type);

		static uint32_t GetGLTextureWrap(TextureWrap wrap);
		static uint32_t GetGLTextureMinFilter(TextureFilterMin filter);
		static uint32_t GetGLTextureMagFilter(TextureFilterMag filter);
		static uint32_t GetGLTextureInternalFmt(TextureInternalFmt fmt);
		static uint32_t GetGLTextureFmt(TextureFmt fmt);
		static uint32_t GetGLTexturePixelType(PixelType type);
		static uint32_t GetGLDepthAttachment(DepthAttachment attach);

	private:
		static bool CreateShader(uint32_t type, const char *source, uint32_t &shader);
		static void LogShaderInfo(uint32_t shader);
		static void LogLinkStatus(uint32_t program);
	};
}

#endif