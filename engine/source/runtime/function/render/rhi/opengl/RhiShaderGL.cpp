#include "function/render/rhi/opengl/RhiShaderGL.h"
#include "function/render/rhi/RhiHeader.h"

namespace Pionner
{
	RhiShaderGL::RhiShaderGL()
	{
	}

	RhiShaderGL::~RhiShaderGL()
	{
	}

	bool RhiShaderGL::initialize()
	{
		return false;
	}

	void RhiShaderGL::activate(bool active)
	{
	}

	bool RhiShaderGL::shutdown()
	{
		return false;
	}

	bool RhiShaderGL::isInit() const
	{
		return false;
	}

	void RhiShaderGL::setInt(const std::string &name, int val) const
	{
	}

	void RhiShaderGL::setFloat(const std::string &name, float val) const
	{
	}

	void RhiShaderGL::setMat3(const std::string &name, const glm::mat3 &val) const
	{
	}

	void RhiShaderGL::setMat4(const std::string &name, const glm::mat4 &val) const
	{
	}
	void RhiShaderGL::setVec2(const std::string &name, const glm::vec2 &val) const
	{
	}

	void RhiShaderGL::setVec3(const std::string &name, const glm::vec3 &val) const
	{
	}

	void RhiShaderGL::setVec4(const std::string &name, const glm::vec4 &val) const
	{
	}
}