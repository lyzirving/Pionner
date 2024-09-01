#include <cstdio>
#include <stdarg.h>

#include "GLHelper.h"
#include "GLHeader.h"

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

	uint32_t GLHelper::CreateSimpleProgram(const uint32_t stageNum, const std::initializer_list<std::string>& stageSource)
	{
		if (stageNum <= 1)
		{
			LOGE("Err! Invalid stage source, stage num is 0");
			return 0;
		}
		const char* vert{ nullptr }, * frag{ nullptr }, * geo{ nullptr };
		auto it = stageSource.begin();
		uint32_t stageIdx{ 0 };
		bool next{ true };
		while (it != stageSource.end() && next)
		{
			switch (stageIdx)
			{
				case 0:
				{
					next = !it->empty() && (vert = it->c_str());
					if (next) { stageIdx++; }
					break;
				}
				case 1:
				{
					next = !it->empty() && (frag = it->c_str());
					if (next) { stageIdx++; }
					break;
				}
				case 2:
				{
					next = !it->empty() && (geo = it->c_str());
					if (next) { stageIdx++; }
					break;
				}
				default:
					LOGE("invalid stage idx[%u]", stageIdx);
					assert(0);
					break;
			}
			it++;
		}

		if (stageNum != stageIdx)
		{
			LOGE("fail to acquire stage source, current[%u], required[%u]", stageIdx - 1, stageNum);
			return 0;
		}

		GLuint vertShader{ 0 }, fragShader{ 0 }, geoShader{ 0 }, program{ 0 };
		GLint linkStatus{ GL_FALSE };
		bool needGeo = stageIdx > 2;

		if (!vert || !CreateShader(GL_VERTEX_SHADER, vert, vertShader))
		{
			LOGE("fail to create vertex shader:\n %s", vert);
			goto err;
		}

		if (!frag || !CreateShader(GL_FRAGMENT_SHADER, frag, fragShader))
		{
			LOGE("fail to create fragment shader:\n %s", frag);
			goto err;
		}

		if (needGeo && (!geo || !CreateShader(GL_GEOMETRY_SHADER, geo, geoShader)))
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

		if (needGeo)
		{
			glAttachShader(program, geoShader);
			if (!CheckError("fail to attach geometry shader"))
				goto err;
		}

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
		if (geoShader) { glDeleteShader(geoShader); }
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

	uint32_t GLHelper::CreateShaderProgram(const char* vert, const char* frag)
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

	uint32_t GLHelper::CreateShaderProgram(const char* vert, const char* frag, const char* geo)
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
			default:
				LOGE("invalid shader data type[%u]", type);
				std::abort();
				return 0;
		}
	}

	uint32_t GLHelper::GetTextureWrap(TextureWrap wrap)
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
				LOGE("err! invalid wrap[%u]", wrap);
				std::abort();
				return GL_NONE;
		}
	}

	uint32_t GLHelper::GetTextureMinFilter(TextureFilterMin filter)
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
			{
				LOGE("err! invalid min filter[%u]", filter);
				std::abort();
				return GL_NONE;
			}
		}
	}

	uint32_t GLHelper::GetTextureMagFilter(TextureFilterMag filter)
	{
		switch (filter)
		{
			case TextureFilterMag::Linear:
				return GL_LINEAR;
			case TextureFilterMag::Nearest:
				return GL_NEAREST;
			default:
				LOGE("err! invalid max filter[%u]", filter);
			std:abort();
				return GL_NONE;
		}
	}

	uint32_t GLHelper::GetTextureInternalFmt(TextureFormat fmt)
	{
		switch (fmt)
		{
			case TextureFormat::RGBA_32:
				return GL_RGBA;
			case TextureFormat::RGB_24:
				return GL_RGB;
			case TextureFormat::RGB_16:
				return GL_RGB16UI;
			case TextureFormat::R_16:
				return GL_R16UI;
			case TextureFormat::R_8:
			case TextureFormat::Alpha_8:
				return GL_R8;
			case TextureFormat::RGBA_FLOAT:
				return GL_RGBA32F;
			case TextureFormat::RGBA_HALF:
				return GL_RGBA16F;
			case TextureFormat::RG_FLOAT:
				return GL_RG32F;
			case TextureFormat::R_FLOAT:
				return GL_R32F;
			case TextureFormat::DEPTH_16:
				return GL_DEPTH_COMPONENT16;
			case TextureFormat::DEPTH_24:
				return GL_DEPTH_COMPONENT24;
			case TextureFormat::DEPTH_32:
				return GL_DEPTH_COMPONENT32;
			case TextureFormat::DEPTH_32F:
				return GL_DEPTH_COMPONENT32F;
			case TextureFormat::DEPTH_24_STENCIL_8:
				return GL_DEPTH24_STENCIL8;
			default:
				LOGE("err! invalid texture format[%u]", fmt);
				std::abort();
				return GL_NONE;
		}
	}

	uint32_t GLHelper::GetTexturePixelFmt(TextureFormat fmt)
	{
		switch (fmt)
		{
			case TextureFormat::RGBA_32:
			case TextureFormat::RGBA_FLOAT:
			case TextureFormat::RGBA_HALF:
				return GL_RGBA;
			case TextureFormat::RGB_24:
			case TextureFormat::RGB_16:
				return GL_RGB;
			case TextureFormat::R_FLOAT:
			case TextureFormat::R_16:
			case TextureFormat::R_8:
			case TextureFormat::Alpha_8:
				return GL_RED;
			case TextureFormat::RG_FLOAT:
				return GL_RG;
			case TextureFormat::DEPTH_32F:
			case TextureFormat::DEPTH_32:
			case TextureFormat::DEPTH_24:
			case TextureFormat::DEPTH_16:
				return GL_DEPTH_COMPONENT;
			case TextureFormat::DEPTH_24_STENCIL_8:
				return GL_DEPTH_STENCIL;
			default:
				LOGE("err! invalid texture format[%u]", fmt);
				std::abort();
				return GL_NONE;
		}
	}

	uint32_t GLHelper::GetTexturePixelType(TextureFormat fmt)
	{
		switch (fmt)
		{
			case TextureFormat::RGBA_32:
			case TextureFormat::RGB_24:
			case TextureFormat::R_8:
			case TextureFormat::Alpha_8:
				return GL_UNSIGNED_BYTE;
			case TextureFormat::RGB_16:
			case TextureFormat::R_16:
				return GL_UNSIGNED_SHORT;
			case TextureFormat::RGBA_FLOAT:
			case TextureFormat::RG_FLOAT:
			case TextureFormat::R_FLOAT:
			case TextureFormat::DEPTH_32F:
				return GL_FLOAT;
			case TextureFormat::RGBA_HALF:
				return GL_HALF_FLOAT;
			case TextureFormat::DEPTH_16:
				return GL_UNSIGNED_SHORT;
			case TextureFormat::DEPTH_24:
			case TextureFormat::DEPTH_32:
				return GL_UNSIGNED_INT;
			case TextureFormat::DEPTH_24_STENCIL_8:
				return GL_UNSIGNED_INT_24_8;
			default:
				LOGE("err! invalid texture format[%u]", fmt);
				std::abort();
				return GL_NONE;
		}
	}

	uint32_t GLHelper::GetDepthAttachment(TextureFormat fmt)
	{
		switch (fmt)
		{			
			case TextureFormat::DEPTH_16:
			case TextureFormat::DEPTH_24:
			case TextureFormat::DEPTH_32:
			case TextureFormat::DEPTH_32F:
				return GL_DEPTH_ATTACHMENT;
			case TextureFormat::DEPTH_24_STENCIL_8:
				return GL_DEPTH_STENCIL_ATTACHMENT;
			default:
				LOGE("err! invalid texture format[%u]", fmt);
				std::abort();
				return GL_NONE;
		}
	}

	bool GLHelper::CreateShader(uint32_t type, const char* source, uint32_t& shader)
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
			char* info = new char[infoLen + 1];
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
		char* info = new char[infoLen];
		std::memset(info, 0, infoLen * sizeof(char));

		glGetProgramInfoLog(program, infoLen, nullptr, info);
		LOGE("program[%u] link status: %s", program, info);
		delete[] info;
	}
}