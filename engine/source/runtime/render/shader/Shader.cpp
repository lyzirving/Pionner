#include <iostream>
#include <fstream>
#include <sstream>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "render/rhi/RhiHeader.h"

#include "core/log/LogSystem.h"
#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Shader"

namespace pio
{
	Shader::Shader(const char *name, const char *vertName, const char *fragName)
		: m_name(name)
	{
		std::ifstream vertFile, fragFile;
		std::stringstream vertStream, fragStream;

		std::string root{ "shader/" };
		std::string vertPath = root + vertName + ".vert";
		std::string fragPath = root + fragName + ".frag";

		vertFile.open(vertPath.c_str());
		if (!vertFile.is_open())
		{
			LOG_ERR("fail to open vertex shader file[%s]", vertPath.c_str());
			return;
		}

		fragFile.open(fragPath.c_str());
		if (!fragFile.is_open())
		{
			LOG_ERR("fail to open fragment shader file[%s]", fragPath.c_str());
			vertFile.close();
			return;
		}

		vertStream << vertFile.rdbuf();
		fragStream << fragFile.rdbuf();

		vertFile.close();
		fragFile.close();

		m_vert = vertStream.str();
		m_frag = fragStream.str();
		LOG_DEBUG("succeed to open shader[%s]\n", m_name.c_str());
	}

	Shader::Shader(const char *name, const char *vertName, const char *fragName, const char *geoName)
		: Shader(name, vertName, fragName)
	{
		std::ifstream geoFile;
		std::stringstream geoStream;

		std::string root{ "shader/" };
		std::string geoPath = root + geoName + ".geom";

		geoFile.open(geoPath.c_str());
		if (!geoFile.is_open())
		{
			LOG_ERR("fail to open geometry shader file[%s]", geoPath.c_str());
			return;
		}

		geoStream << geoFile.rdbuf();

		geoFile.close();

		m_geo = geoStream.str();

		LOG_DEBUG("succeed to open geometry shader[%s]\n", m_name.c_str());
	}

	Shader::~Shader()
	{
		destroy();
	}

	bool Shader::init()
	{
		if (isInit())
			return true;

		if (m_vert.empty() || m_frag.empty())
		{
			LOG_ERR("source is invalid");
			return false;
		}

		if (!m_geo.empty())
		{
			m_program = gfx::GLHelper::buildProgram(m_vert.c_str(), m_frag.c_str(), m_geo.c_str());
		}
		else
		{
			m_program = gfx::GLHelper::buildProgram(m_vert.c_str(), m_frag.c_str());
		}
		
		bool success = m_program > 0;
		if (success)
		{
			LOG_DEBUG("succeed to init shader[%s][%u]", m_name.c_str(), m_program);
		}

		return success;
	}

	void Shader::destroy()
	{
		if (isInit())
		{
			glDeleteProgram(m_program);
			m_program = 0;
		}
	}

	void Shader::use(bool active)
	{
		if (isInit())
		{
			glUseProgram(active ? m_program : 0);
			gfx::GLHelper::checkGLErr("call activate err, program[%u], active[%s]",
								 m_program, active ? "true" : "false");
		}
	}

	void Shader::setFloat(const std::string &name, float value) 
	{
		if (isInit())
		{
			GLint ind = glGetUniformLocation(m_program, name.c_str());
			glUniform1f(ind, value);
			gfx::GLHelper::checkGLErr("setFloat err, program[%u], name[%s], val[%f]",
								 m_program, name.c_str(), value);
		}
	}

	void Shader::setInt(const std::string &name, int value)
	{
		if (isInit())
		{
			GLint ind = glGetUniformLocation(m_program, name.c_str());
			glUniform1i(ind, value);
			gfx::GLHelper::checkGLErr("setInt err, program[%u], name[%s], val[%d]",
								 m_program, name.c_str(), value);
		}
	}

	void Shader::setMat3(const std::string &name, const glm::mat3 &mat)
	{
		if (isInit())
		{
			GLint ind = glGetUniformLocation(m_program, name.c_str());
			glUniformMatrix3fv(ind, 1, GL_FALSE, glm::value_ptr(mat));
			gfx::GLHelper::checkGLErr("setMat3 err, program[%u], name[%s]", m_program, name.c_str());
		}
	}

	void Shader::setMat4(const std::string &name, const glm::mat4 &mat)
	{
		if (isInit())
		{
			GLint ind = glGetUniformLocation(m_program, name.c_str());
			glUniformMatrix4fv(ind, 1, GL_FALSE, glm::value_ptr(mat));
			gfx::GLHelper::checkGLErr("setMat4 err, program[%u], name[%s]", m_program, name.c_str());
		}
	}

	void Shader::setVec2(const std::string &name, const glm::vec2 &val)
	{
		if (isInit())
		{
			GLint ind = glGetUniformLocation(m_program, name.c_str());
			glUniform2f(ind, val.x, val.y);
			gfx::GLHelper::checkGLErr("setVec2 err, program[%u], name[%s], val[%f, %f]",
								 m_program, name.c_str(), val.x, val.y);
		}
	}

	void Shader::setVec2(const std::string &name, float val0, float val1)
	{
		if (isInit())
		{
			GLint ind = glGetUniformLocation(m_program, name.c_str());
			glUniform2f(ind, val0, val1);
			gfx::GLHelper::checkGLErr("setVec2 err, program[%u], name[%s], val[%f, %f]",
								 m_program, name.c_str(), val0, val1);
		}
	}

	void Shader::setVec3(const std::string &name, const glm::vec3 &val)
	{
		if (isInit())
		{
			GLint ind = glGetUniformLocation(m_program, name.c_str());
			glUniform3f(ind, val.x, val.y, val.z);
			gfx::GLHelper::checkGLErr("setVec3 err, program[%u], name[%s], val[%f, %f, %f]",
								 m_program, name.c_str(), val.x, val.y, val.z);
		}
	}

	void Shader::setVec3(const std::string &name, float val0, float val1, float val2) 
	{
		if (isInit())
		{
			GLint ind = glGetUniformLocation(m_program, name.c_str());
			glUniform3f(ind, val0, val1, val2);
			gfx::GLHelper::checkGLErr("setVec3 err, program[%u], name[%s], val[%f, %f, %f]",
								 m_program, name.c_str(), val0, val1, val2);
		}
	}

	void Shader::setVec4(const std::string &name, const glm::vec4 &val)
	{
		if (isInit())
		{
			GLint ind = glGetUniformLocation(m_program, name.c_str());
			glUniform4f(ind, val.x, val.y, val.z, val.w);
			gfx::GLHelper::checkGLErr("setVec4 err, program[%u], name[%s], val[%f, %f, %f£¬ %f]",
								 m_program, name.c_str(), val.x, val.y, val.z, val.w);
		}
	}

	void Shader::setVec4(const std::string &name, float val0, float val1, float val2, float val3)
	{
		if (isInit())
		{
			GLint ind = glGetUniformLocation(m_program, name.c_str());
			glUniform4f(ind, val0, val1, val2, val3);
			gfx::GLHelper::checkGLErr("setVec4 err, program[%u], name[%s], val[%f, %f, %f£¬ %f]",
								 m_program, name.c_str(), val0, val1, val2, val3);
		}
	}

}