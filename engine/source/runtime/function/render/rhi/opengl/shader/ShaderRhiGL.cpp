#include <glm/gtc/type_ptr.hpp>

#include "function/render/rhi/opengl/shader/ShaderRhiGL.h"
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
			GLHelper::checkGLErr("call activate err, program[%u], active[%s]",
								 program, active ? "true" : "false");
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

	void ShaderRhiGL::setInt(uint32_t program, const std::string &name, int val) const
	{
		if (isInit(program))
		{
			GLint ind = glGetUniformLocation(program, name.c_str());
			glUniform1i(ind, val);
			GLHelper::checkGLErr("setInt err, program[%u], name[%s], val[%d]",
								 program, name.c_str(), val);
		}
	}

	void ShaderRhiGL::setFloat(uint32_t program, const std::string &name, float val) const
	{
		if (isInit(program))
		{
			GLint ind = glGetUniformLocation(program, name.c_str());
			glUniform1f(ind, val);
			GLHelper::checkGLErr("setFloat err, program[%u], name[%s], val[%f]",
								 program, name.c_str(), val);
		}
	}

	void ShaderRhiGL::setMat3(uint32_t program, const std::string &name, const glm::mat3 &val) const
	{
		if (isInit(program))
		{
			GLint ind = glGetUniformLocation(program, name.c_str());
			glUniformMatrix3fv(ind, 1, GL_FALSE, glm::value_ptr(val));
			GLHelper::checkGLErr("setMat3 err, program[%u], name[%s]", program, name.c_str());
		}
	}

	void ShaderRhiGL::setMat4(uint32_t program, const std::string &name, const glm::mat4 &val) const
	{
		if (isInit(program))
		{
			GLint ind = glGetUniformLocation(program, name.c_str());
			glUniformMatrix4fv(ind, 1, GL_FALSE, glm::value_ptr(val));
			GLHelper::checkGLErr("setMat4 err, program[%u], name[%s]", program, name.c_str());
		}
	}

	void ShaderRhiGL::setVec2(uint32_t program, const std::string &name, const glm::vec2 &val) const
	{
		if (isInit(program))
		{
			GLint ind = glGetUniformLocation(program, name.c_str());
			glUniform2f(ind, val.x, val.y);
			GLHelper::checkGLErr("setVec2 err, program[%u], name[%s], val[%f, %f]",
								 program, name.c_str(), val.x, val.y);
		}
	}

	void ShaderRhiGL::setVec2(uint32_t program, const std::string &name, float val0, float val1) const
	{
		if (isInit(program))
		{
			GLint ind = glGetUniformLocation(program, name.c_str());
			glUniform2f(ind, val0, val1);
			GLHelper::checkGLErr("setVec2 err, program[%u], name[%s], val[%f, %f]",
								 program, name.c_str(), val0, val1);
		}
	}

	void ShaderRhiGL::setVec3(uint32_t program, const std::string &name, const glm::vec3 &val) const
	{
		if (isInit(program))
		{
			GLint ind = glGetUniformLocation(program, name.c_str());
			glUniform3f(ind, val.x, val.y, val.z);
			GLHelper::checkGLErr("setVec3 err, program[%u], name[%s], val[%f, %f, %f]",
								 program, name.c_str(), val.x, val.y, val.z);
		}
	}

	void ShaderRhiGL::setVec3(uint32_t program, const std::string &name, float val0, float val1, float val2) const
	{
		if (isInit(program))
		{
			GLint ind = glGetUniformLocation(program, name.c_str());
			glUniform3f(ind, val0, val1, val2);
			GLHelper::checkGLErr("setVec3 err, program[%u], name[%s], val[%f, %f, %f]",
								 program, name.c_str(), val0, val1, val2);
		}
	}

	void ShaderRhiGL::setVec4(uint32_t program, const std::string &name, const glm::vec4 &val) const
	{
		if (isInit(program))
		{
			GLint ind = glGetUniformLocation(program, name.c_str());
			glUniform4f(ind, val.x, val.y, val.z, val.w);
			GLHelper::checkGLErr("setVec4 err, program[%u], name[%s], val[%f, %f, %f£¬ %f]",
								 program, name.c_str(), val.x, val.y, val.z, val.w);
		}
	}

	void ShaderRhiGL::setVec4(uint32_t program, const std::string &name, float val0, float val1, float val2, float val3) const
	{
		if (isInit(program))
		{
			GLint ind = glGetUniformLocation(program, name.c_str());
			glUniform4f(ind, val0, val1, val2, val3);
			GLHelper::checkGLErr("setVec4 err, program[%u], name[%s], val[%f, %f, %f£¬ %f]",
								 program, name.c_str(), val0, val1, val2, val3);
		}
	}
}