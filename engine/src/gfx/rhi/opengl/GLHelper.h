#ifndef __PIONNER_GFX_RHI_OPENGL_GL_HELPER_H__
#define __PIONNER_GFX_RHI_OPENGL_GL_HELPER_H__

#include "gfx/GfxDef.h"

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
		static uint32_t CreateSimpleProgram(const uint32_t stageNum, const std::initializer_list<std::string>& stageSource);
		static uint32_t CreateShaderProgram(const char* vert, const char* frag);
		static uint32_t CreateShaderProgram(const char* vert, const char* frag, const char* geo);
		static uint32_t ShaderDataTypeToGLBaseType(ShaderDataType type);

		static uint32_t GetTextureWrap(TextureWrap wrap);
		static uint32_t GetTextureMinFilter(TextureFilterMin filter);
		static uint32_t GetTextureMagFilter(TextureFilterMag filter);
		//Specifies the number of color components in the texture.
		static uint32_t GetTextureInternalFmt(TextureFormat fmt);
		//Specifies the format of the pixel data.
		static uint32_t GetTexturePixelFmt(TextureFormat fmt);
		//Specifies the data type of the pixel data
		static uint32_t GetTexturePixelType(TextureFormat fmt);
		static uint32_t GetDepthAttachment(TextureFormat fmt);

		static uint32_t GetDrawMode(DrawMode mode);
		static uint32_t GetIndexInternalFormat(IndexInternalFmt fmt);

	private:
		static bool CreateShader(uint32_t type, const char* source, uint32_t& shader);
		static void LogShaderInfo(uint32_t shader);
		static void LogLinkStatus(uint32_t program);
	};
}

#endif