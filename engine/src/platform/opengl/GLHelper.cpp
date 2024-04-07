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

	bool GLHelper::CheckError(const char *fmt, ...)
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

	bool GLHelper::CheckFrameBufferStatus(const char *fmt, ...)
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

	uint32_t GLHelper::CreateShaderProgram(const char *vert, const char *frag)
	{
		GLuint vertShader{ 0 };
		GLuint fragShader{ 0 };
		GLuint program{ 0 };
		GLint  linkStatus{ GL_FALSE };

		if (vert == nullptr || std::strlen(vert) == 0)
		{
			LOGE("input vertex shader is null");
			goto err;
		}

		if (frag == nullptr || std::strlen(frag) == 0)
		{
			LOGE("input fragment shader is null");
			goto err;
		}

		if (!CreateShader(GL_VERTEX_SHADER, vert, vertShader))
		{
			LOGE("fail to create vertex shader:\n %s", vert);
			goto err;
		}

		if (!CreateShader(GL_FRAGMENT_SHADER, frag, fragShader))
		{
			LOGE("fail to create fragment shader:\n %s", frag);
			goto err;
		}

		program = glCreateProgram();
		if (!program)
		{
			CheckError("fail to create program");
			goto err;
		}

		glAttachShader(program, vertShader);
		if (!CheckError("fail to attach vertex shader"))
			goto err;

		glAttachShader(program, fragShader);
		if (!CheckError("fail to attach fragment shader"))
			goto err;

		glLinkProgram(program);
		glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
		if (linkStatus != GL_TRUE)
		{
			LOGE("fail to link program[%u]", program);
			LogLinkStatus(program);
			goto err;
		}

		// work is done, delete created shaders
		glDeleteShader(vertShader);
		glDeleteShader(fragShader);

		return program;

	err:
		if (program)
			glDeleteProgram(program);

		if (vertShader)
			glDeleteShader(vertShader);

		if (fragShader)
			glDeleteShader(fragShader);
		return 0;
	}

	uint32_t GLHelper::CreateShaderProgram(const char *vert, const char *frag, const char *geo)
	{
		GLuint vertShader{ 0 }, fragShader{ 0 }, geoShader{ 0 };
		GLuint program{ 0 };
		GLint  linkStatus{ GL_FALSE };

		if (vert == nullptr || std::strlen(vert) == 0)
		{
			LOGE("input vertex shader is null");
			goto err;
		}

		if (frag == nullptr || std::strlen(frag) == 0)
		{
			LOGE("input fragment shader is null");
			goto err;
		}

		if (geo == nullptr || std::strlen(geo) == 0)
		{
			LOGE("input geometry shader is null");
			goto err;
		}

		if (!CreateShader(GL_VERTEX_SHADER, vert, vertShader))
		{
			LOGE("fail to create vertex shader:\n %s", vert);
			goto err;
		}

		if (!CreateShader(GL_FRAGMENT_SHADER, frag, fragShader))
		{
			LOGE("fail to create fragment shader:\n %s", frag);
			goto err;
		}

		if (!CreateShader(GL_GEOMETRY_SHADER, geo, geoShader))
		{
			LOGE("fail to create geometry shader:\n %s", geo);
			goto err;
		}

		program = glCreateProgram();
		if (!program)
		{
			CheckError("fail to create program");
			goto err;
		}

		glAttachShader(program, vertShader);
		if (!CheckError("fail to attach vertex shader"))
			goto err;

		glAttachShader(program, fragShader);
		if (!CheckError("fail to attach fragment shader"))
			goto err;

		glAttachShader(program, geoShader);
		if (!CheckError("fail to attach geometry shader"))
			goto err;

		glLinkProgram(program);
		glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
		if (linkStatus != GL_TRUE)
		{
			LOGE("fail to link program[%u]", program);
			LogLinkStatus(program);
			goto err;
		}

		// work is done, delete created shaders
		glDeleteShader(vertShader);
		glDeleteShader(fragShader);
		glDeleteShader(geoShader);

		return program;

	err:
		if (program)
			glDeleteProgram(program);

		if (vertShader)
			glDeleteShader(vertShader);

		if (fragShader)
			glDeleteShader(fragShader);

		if (geoShader)
			glDeleteShader(geoShader);

		return 0;
	}

	uint32_t GLHelper::ShaderDataTypeToGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::Byte:
			case ShaderDataType::Byte2:
			case ShaderDataType::Byte3:
			case ShaderDataType::Byte4:
				return GL_BYTE;
			case ShaderDataType::UByte:
			case ShaderDataType::UByte2:
			case ShaderDataType::UByte3:
			case ShaderDataType::UByte4:
				return GL_UNSIGNED_BYTE;
			case ShaderDataType::Short:
			case ShaderDataType::Short2:
			case ShaderDataType::Short3:
			case ShaderDataType::Short4:
				return GL_SHORT;
			case ShaderDataType::UShort:
			case ShaderDataType::UShort2:
			case ShaderDataType::UShort3:
			case ShaderDataType::UShort4:
				return GL_UNSIGNED_SHORT;
			case ShaderDataType::Int:
			case ShaderDataType::Int2:
			case ShaderDataType::Int3:
			case ShaderDataType::Int4:
				return GL_INT;
			case ShaderDataType::UInt:
			case ShaderDataType::UInt2:
			case ShaderDataType::UInt3:
			case ShaderDataType::UInt4:
				return GL_UNSIGNED_INT;
			case ShaderDataType::Float:
			case ShaderDataType::Float2:
			case ShaderDataType::Float3:
			case ShaderDataType::Float4:
				return GL_FLOAT;
			case ShaderDataType::Double:
			case ShaderDataType::Double2:
			case ShaderDataType::Double3:
			case ShaderDataType::Double4:
				return GL_DOUBLE;
			case ShaderDataType::None:
			default:
				LOGE("invalid shader data type[%u]", type);
				return 0;
		}
	}

	uint32_t GLHelper::GetGLTextureWrap(TextureWrap wrap)
	{
		switch (wrap)
		{
			case TextureWrap::ClampEdge:
				return GL_CLAMP_TO_EDGE;
			case TextureWrap::ClampBorder:
				return GL_CLAMP_TO_BORDER;
			case TextureWrap::Repeat:
				return GL_REPEAT;
			default:
				LOGE("invalid wrap[%u]", wrap);
				return GL_NONE;
		}
	}

	uint32_t GLHelper::GetGLTextureMinFilter(TextureFilterMin filter)
	{
		switch (filter)
		{
			case TextureFilterMin::Linear:
				return GL_LINEAR;
			case TextureFilterMin::Nearest:
				return GL_NEAREST;
			case TextureFilterMin::Linear_Mipmap_Linear:
				return GL_LINEAR_MIPMAP_LINEAR;
			case TextureFilterMin::Linear_Mipmap_Nearest:
				return GL_LINEAR_MIPMAP_NEAREST;
			case TextureFilterMin::Nearest_Mipmap_Linear:
				return GL_NEAREST_MIPMAP_LINEAR;
			default:
				LOGE("invalid min filter[%u]", filter);
				return GL_NONE;
		}
	}

	uint32_t GLHelper::GetGLTextureMagFilter(TextureFilterMag filter)
	{
		switch (filter)
		{
			case TextureFilterMag::Linear:
				return GL_LINEAR;
			case TextureFilterMag::Nearest:
				return GL_NEAREST;
			default:
				LOGE("invalid max filter[%u]", filter);
				return GL_NONE;
		}
	}

	uint32_t GLHelper::GetGLTextureInternalFmt(TextureInternalFmt fmt)
	{
		switch (fmt)
		{
			case TextureInternalFmt::Depth:
				return GL_DEPTH_COMPONENT;
			case TextureInternalFmt::Depth24:
				return GL_DEPTH_COMPONENT24;
			case TextureInternalFmt::Depth32F:
				return GL_DEPTH_COMPONENT32F;
			case TextureInternalFmt::DepthStencil:
				return GL_DEPTH24_STENCIL8;		
			case TextureInternalFmt::RED:
				return GL_RED;
			case TextureInternalFmt::R8:
				return GL_R8;
			case TextureInternalFmt::R8U:
				return GL_R8UI;
			case TextureInternalFmt::R16U:
				return GL_R16UI;
			case TextureInternalFmt::R32U:
				return GL_R32UI;
			case TextureInternalFmt::R32F:
				return GL_R32F;
			case TextureInternalFmt::RG:
				return GL_RG;
			case TextureInternalFmt::RG8:
				return GL_RG8;
			case TextureInternalFmt::RG8U:
				return GL_RG8UI;
			case TextureInternalFmt::RG16F:
				return GL_RG16F;
			case TextureInternalFmt::RG32F:
				return GL_RG32F;
			case TextureInternalFmt::RGB:
				return GL_RGB;
			case TextureInternalFmt::RGB8U:
				return GL_RGB8UI;
			case TextureInternalFmt::RGB16F:
				return GL_RGB16F;
			case TextureInternalFmt::RGBUI:
				return GL_RGB32UI;
			case TextureInternalFmt::RGB32F:
				return GL_RGB32F;
			case TextureInternalFmt::RGBA:
				return GL_RGBA;
			case TextureInternalFmt::RGBAUI:
				return GL_RGBA32UI;
			case TextureInternalFmt::RGBA8U:
				return GL_RGBA8UI;
			case TextureInternalFmt::RGBA16U:
				return GL_RGBA16UI;
			case TextureInternalFmt::RGBA16F:
				return GL_RGBA16F;
			case TextureInternalFmt::RGBA32F:
				return GL_RGBA32F;
			default:
				LOGE("invalid internal fmt[%u]", fmt);
				return GL_NONE;
		}
	}

	uint32_t GLHelper::GetGLTextureFmt(TextureFmt fmt)
	{
		switch (fmt)
		{
			case TextureFmt::Depth:
				return GL_DEPTH_COMPONENT;
			case TextureFmt::DepthStencil:
				return GL_DEPTH_STENCIL;
			case TextureFmt::RED:
				return GL_RED;
			case TextureFmt::RG:
				return GL_RG;
			case TextureFmt::RGB:
				return GL_RGB;
			case TextureFmt::BGR:
				return GL_BGR;
			case TextureFmt::RGBA:
				return GL_RGBA;
			case TextureFmt::BGRA:
				return GL_BGRA;
			case TextureFmt::REDI:
				return GL_RED_INTEGER;
			case TextureFmt::RGI:
				return GL_RG_INTEGER;
			case TextureFmt::RGBI:
				return GL_RGB_INTEGER;
			case TextureFmt::BGRI:
				return GL_BGR_INTEGER;
			case TextureFmt::RGBAI:
				return GL_RGBA_INTEGER;
			case TextureFmt::BGRAI:
				return GL_BGRA_INTEGER;
			default:
				LOGE("invalid texture fmt[%u]", fmt);
				return GL_NONE;
		}
	}

	uint32_t GLHelper::GetGLTexturePixelType(PixelType type)
	{
		switch (type)
		{
			case PixelType::UNSIGNED_BYTE:
				return GL_UNSIGNED_BYTE;
			case PixelType::BYTE:
				return GL_BYTE;
			case PixelType::UNSIGNED_SHORT:
				return GL_UNSIGNED_SHORT;
			case PixelType::SHORT:
				return GL_SHORT;
			case PixelType::UNSIGNED_INT:
				return GL_UNSIGNED_INT;
			case PixelType::UNSIGNED_INT_24_8:
				return GL_UNSIGNED_INT_24_8;				
			case PixelType::INT:
				return GL_INT;
			case PixelType::HALF_FLOAT:
				return GL_HALF_FLOAT;
			case PixelType::FLOAT:
				return GL_FLOAT;
			default:
				LOGE("invalid pixel type[%u]", type);
				return GL_NONE;
		}
	}

	uint32_t GLHelper::GetGLDepthAttachment(DepthAttachment attach)
	{
		switch (attach)
		{
			case DepthAttachment::Depth:
				return GL_DEPTH_ATTACHMENT;
			case DepthAttachment::DepthStencil:
				return GL_DEPTH_STENCIL_ATTACHMENT;
			default:
				LOGE("invalid depth attachment[%u]", attach);
				return GL_NONE;
		}
	}

	bool GLHelper::CreateShader(uint32_t type, const char *source, uint32_t &shader)
	{
		shader = glCreateShader(type);
		if (shader)
		{
			glShaderSource(shader, 1, &source, nullptr);
			glCompileShader(shader);
			GLint compileStatus = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
			if (compileStatus != GL_TRUE)
			{
				LogShaderInfo(shader);
				glDeleteShader(shader);
				shader = 0;
			}
		}
		else
		{
			LOGE("fail to create shader for type[0x%x], err code[0x%x]", type, glGetError());
		}
		return shader > 0;
	}

	void GLHelper::LogShaderInfo(uint32_t shader)
	{
		if (shader <= 0)
		{
			LOGE("input shader is invalid");
			return;
		}
		GLint infoLen{ 0 };
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
		if (infoLen)
		{
			char *info = new char[infoLen + 1];
			std::memset(info, 0, (infoLen + 1) * sizeof(char));

			glGetShaderInfoLog(shader, infoLen, nullptr, info);
			LOGE("shader[%u]: %s", shader, info);
			delete[] info;
		}
		else
		{
			LOGE("no log for shader");
		}
	}

	void GLHelper::LogLinkStatus(uint32_t program)
	{
		if (program <= 0)
		{
			LOGE("input program is invalid");
			return;
		}
		GLint infoLen{ 512 };
		char *info = new char[infoLen];
		std::memset(info, 0, infoLen * sizeof(char));

		glGetProgramInfoLog(program, infoLen, nullptr, info);
		LOGE("program[%u] link status: %s", program, info);
		delete[] info;
	}
}