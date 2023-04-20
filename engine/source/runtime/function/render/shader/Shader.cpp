#include <iostream>
#include <fstream>
#include <sstream>

#include "function/render/shader/Shader.h"
#include "function/render/rhi/RhiShader.h"

#include "core/log/LogSystem.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Shader"

namespace Pionner
{
	Shader::Shader(const std::shared_ptr<RhiShader> &rhi, const char *name, const char *vertName, const char *fragName)
		: m_rhi(rhi), m_name(name)
		, m_vert(), m_frag(), m_program(0)
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
		LOG_INFO("succeed to open shader[%s]", m_name.c_str());
	}

	Shader::~Shader()
	{
		if (m_rhi)
			m_rhi->destroy(m_program);
		m_rhi.reset();
	}

	bool Shader::init()
	{
		if (!m_rhi)
			return false;

		if (isInit())
			return true;

		if (m_vert.empty() || m_frag.empty())
		{
			LOG_ERR("source is invalid");
			return false;
		}

		return m_rhi->build(m_vert.c_str(), m_frag.c_str(), m_program);
	}

	void Shader::use(bool active)
	{
		if (m_rhi)
			m_rhi->activate(m_program, active);
	}

	void Shader::setFloat(const std::string &name, float value) const
	{
		if (m_rhi)
			m_rhi->setFloat(m_program, name, value);
	}

	void Shader::setInt(const std::string &name, int value) const
	{
		if (m_rhi)
			m_rhi->setInt(m_program, name, value);
	}

	void Shader::setMat3(const std::string &name, const glm::mat3 &mat) const
	{
		if (m_rhi)
			m_rhi->setMat3(m_program, name, mat);
	}

	void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const
	{
		if (m_rhi)
			m_rhi->setMat4(m_program, name, mat);
	}

	void Shader::setVec2(const std::string &name, const glm::vec2 &vec2) const
	{
		if (m_rhi)
			m_rhi->setVec2(m_program, name, vec2);
	}

	void Shader::setVec2(const std::string &name, float val0, float val1) const
	{
		if (m_rhi)
			m_rhi->setVec2(m_program, name, val0, val1);
	}

	void Shader::setVec3(const std::string &name, const glm::vec3 &vec3) const
	{
		if (m_rhi)
			m_rhi->setVec3(m_program, name, vec3);
	}

	void Shader::setVec3(const std::string &name, float val0, float val1, float val2) const
	{
		if (m_rhi)
			m_rhi->setVec3(m_program, name, val0, val1, val2);
	}

	void Shader::setVec4(const std::string &name, const glm::vec4 &vec4) const
	{
		if (m_rhi)
			m_rhi->setVec4(m_program, name, vec4);
	}

	void Shader::setVec4(const std::string &name, float x0, float x1, float x2, float x3) const
	{
		if (m_rhi)
			m_rhi->setVec4(m_program, name, x0, x1, x2, x3);
	}

}