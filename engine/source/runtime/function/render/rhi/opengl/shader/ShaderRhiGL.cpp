#include <glm/gtc/type_ptr.hpp>

#include "function/render/rhi/opengl/shader/ShaderRhiGL.h"
#include "function/render/rhi/opengl/GLHelper.h"
#include "function/render/rhi/RhiHeader.h"

#include "core/log/LogSystem.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "ShaderRhiGL"

namespace Pionner
{
	ShaderRhiGL::ShaderRhiGL()
	{
	}

	ShaderRhiGL::~ShaderRhiGL()
	{
	}

	bool ShaderRhiGL::build(const char *vert, const char *frag, uint32_t &program)
	{
		if (vert == nullptr || frag == nullptr)
		{
			LOG_ERR("input src is invalid");
			goto fail;
		}
		program = GLHelper::buildProgram(vert, frag);
		if (program <= 0)
			goto fail;

		return true;

	fail:
		program = 0;
		return false;
	}

	void ShaderRhiGL::activate(uint32_t program, bool active)
	{
		if (isInit(program))
		{
			glUseProgram(active ? program : 0);
		}
	}

	void ShaderRhiGL::destroy(uint32_t &program)
	{
		if (isInit(program))
		{
			glDeleteProgram(program);
			program = 0;
		}
	}

	bool ShaderRhiGL::getAttribLoc(uint32_t program, const std::string &name, int &loc) const
	{
		if (isInit(program))
		{
			loc = glGetAttribLocation(program, name.c_str());
			return GLHelper::checkGLErr("failed to get attribute[%s] location", name.c_str());
		}
		else
		{
			return false;
		}
	}

	void ShaderRhiGL::setInt(uint32_t program, const std::string &name, int val) const
	{
		if (isInit(program))
		{
			GLint ind = glGetUniformLocation(program, name.c_str());
			glUniform1i(ind, val);
		}
	}

	void ShaderRhiGL::setFloat(uint32_t program, const std::string &name, float val) const
	{
		if (isInit(program))
		{
			GLint ind = glGetUniformLocation(program, name.c_str());
			glUniform1f(ind, val);
		}
	}

	void ShaderRhiGL::setMat3(uint32_t program, const std::string &name, const glm::mat3 &val) const
	{
		if (isInit(program))
		{
			GLint ind = glGetUniformLocation(program, name.c_str());
			glUniformMatrix3fv(ind, 1, GL_FALSE, glm::value_ptr(val));
		}
	}

	void ShaderRhiGL::setMat4(uint32_t program, const std::string &name, const glm::mat4 &val) const
	{
		if (isInit(program))
		{
			GLint ind = glGetUniformLocation(program, name.c_str());
			glUniformMatrix4fv(ind, 1, GL_FALSE, glm::value_ptr(val));
		}
	}

	void ShaderRhiGL::setVec2(uint32_t program, const std::string &name, const glm::vec2 &val) const
	{
		if (isInit(program))
		{
			GLint ind = glGetUniformLocation(program, name.c_str());
			glUniform2f(ind, val.x, val.y);
		}
	}

	void ShaderRhiGL::setVec2(uint32_t program, const std::string &name, float val0, float val1) const
	{
		if (isInit(program))
		{
			GLint ind = glGetUniformLocation(program, name.c_str());
			glUniform2f(ind, val0, val1);
		}
	}

	void ShaderRhiGL::setVec3(uint32_t program, const std::string &name, const glm::vec3 &val) const
	{
		if (isInit(program))
		{
			GLint ind = glGetUniformLocation(program, name.c_str());
			glUniform3f(ind, val.x, val.y, val.z);
		}
	}

	void ShaderRhiGL::setVec3(uint32_t program, const std::string &name, float val0, float val1, float val2) const
	{
		if (isInit(program))
		{
			GLint ind = glGetUniformLocation(program, name.c_str());
			glUniform3f(ind, val0, val1, val2);
		}
	}

	void ShaderRhiGL::setVec4(uint32_t program, const std::string &name, const glm::vec4 &val) const
	{
		if (isInit(program))
		{
			GLint ind = glGetUniformLocation(program, name.c_str());
			glUniform4f(ind, val.x, val.y, val.z, val.w);
		}
	}

	void ShaderRhiGL::setVec4(uint32_t program, const std::string &name, float val0, float val1, float val2, float val3) const
	{
		if (isInit(program))
		{
			GLint ind = glGetUniformLocation(program, name.c_str());
			glUniform4f(ind, val0, val1, val2, val3);
		}
	}
}