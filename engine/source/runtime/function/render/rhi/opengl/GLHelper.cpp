#include <string.h>
#include <cstring>
#include <cstdarg>

#include "function/render/rhi/opengl/GLHelper.h"
#include "function/render/rhi/RhiHeader.h"

#include "core/log/LogSystem.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "GLHelper"

namespace Pionner
{
	uint32_t GLHelper::buildProgram(const char *vert, const char *frag)
	{
		GLuint vertShader{ 0 };
		GLuint fragShader{ 0 };
		GLuint program{ 0 };
		GLint linkStatus{ GL_FALSE };

		if (vert == nullptr || std::strlen(vert) == 0)
		{
			LOG_ERR("input vertex shader is null");
			goto err;
		}

		if (frag == nullptr || std::strlen(frag) == 0)
		{
			LOG_ERR("input fragment shader is null");
			goto err;
		}

		if (!createShader(GL_VERTEX_SHADER, vert, vertShader))
		{
			LOG_ERR("fail to create vertex shader:\n %s", vert);
			goto err;
		}

		if (!createShader(GL_FRAGMENT_SHADER, frag, fragShader))
		{
			LOG_ERR("fail to create fragment shader:\n %s", frag);
			goto err;
		}

		program = glCreateProgram();
		if (!program)
		{
			checkGLErr("fail to create program");
			goto err;
		}

		glAttachShader(program, vertShader);
		if (!checkGLErr("fail to attach vertex shader"))
			goto err;

		glAttachShader(program, fragShader);
		if (!checkGLErr("fail to attach fragment shader"))
			goto err;

		glLinkProgram(program);
		glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
		if (linkStatus != GL_TRUE)
		{
			LOG_ERR("fail to link program[%u]", program);
			logLinkStatus(program);
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

	bool GLHelper::checkGLErr(const char *fmt, ...)
	{
		GLenum ret = glGetError();
		if (ret == GL_NO_ERROR)
		{
			return true;
		}
		else
		{
			char buf[512] = { 0 };
			va_list ap;
			va_start(ap, fmt);
			std::vsnprintf(buf, sizeof(buf) - 1, fmt, ap);
			va_end(ap);
			LOG_ERR("%s: err code[0x%x]", buf, ret);
			return false;
		}
	}

	bool GLHelper::createShader(uint32_t type, const char *source, uint32_t &shader)
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
				logShaderInfo(shader);
				glDeleteShader(shader);
				shader = 0;
			}
		}
		else
		{
			LOG_ERR("fail to create shader for type[0x%x], err code[0x%x]", type, glGetError());
		}
		return shader > 0;
	}

	void GLHelper::logShaderInfo(uint32_t shader)
	{
		if (shader <= 0)
		{
			LOG_ERR("input shader is invalid");
			return;
		}
		GLint infoLen{ 0 };
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
		if (infoLen)
		{
			char *info = new char[infoLen + 1];
			std::memset(info, 0, (infoLen + 1) * sizeof(char));

			glGetShaderInfoLog(shader, infoLen, nullptr, info);
			LOG_ERR("shader[%u]: %s", shader, info);
			delete[] info;
		}
		else
		{
			LOG_ERR("no log for shader");
		}
	}

	void GLHelper::logLinkStatus(uint32_t program)
	{
		if (program <= 0)
		{
			LOG_ERR("input program is invalid");
			return;
		}
		GLint infoLen{ 0 };
		glGetProgramiv(program, GL_LINK_STATUS, &infoLen);
		if (infoLen)
		{
			char *info = new char[infoLen + 1];
			std::memset(info, 0, (infoLen + 1) * sizeof(char));

			glGetProgramInfoLog(program, infoLen, nullptr, info);
			LOG_ERR("program[%u] link status: %s", program, info);
			delete[] info;
		}
		else
		{
			LOG_ERR("no log for program's link status");
		}
	}
}


