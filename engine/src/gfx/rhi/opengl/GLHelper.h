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

	private:
		static bool CreateShader(uint32_t type, const char* source, uint32_t& shader);
		static void LogShaderInfo(uint32_t shader);
		static void LogLinkStatus(uint32_t program);
	};
}

#endif