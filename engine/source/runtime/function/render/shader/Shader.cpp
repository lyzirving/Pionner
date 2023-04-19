#include <iostream>
#include <fstream>
#include <sstream>

#include "function/render/shader/Shader.h"
#include "function/render/rhi/RhiHeader.h"

#include "core/log/LogSystem.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Shader"

namespace Pionner
{
	Shader::Shader(const char *name, const char *vertName, const char *fragName)
		: m_name(name), m_vert(), m_frag(), m_program(0)
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

		LOG_INFO("succeed to open shader[%s] file", m_name.c_str());
		m_vert = vertStream.str();
		m_frag = fragStream.str();
	}

	Shader::~Shader()
	{
	}

	void Shader::init()
	{
	}

	void Shader::use(bool active)
	{
	}

	void Shader::setFloat(const std::string &name, float value) const
	{
	}

	void Shader::setInt(const std::string &name, int value) const
	{
	}

	void Shader::setMat3(const std::string &name, const glm::mat3 &mat) const
	{
	}

	void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const
	{
	}

	void Shader::setVec2(const std::string &name, const glm::vec2 &vec2) const
	{
	}

	void Shader::setVec3(const std::string &name, const glm::vec3 &vec3) const
	{
	}

	void Shader::setVec4(const std::string &name, const glm::vec4 &vec4) const
	{
	}

	void Shader::setVec4(const std::string &name, float x0, float x1, float x2, float x3) const
	{
	}

}