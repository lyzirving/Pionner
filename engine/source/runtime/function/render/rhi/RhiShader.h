#ifndef __RHI_SHADER_H__
#define __RHI_SHADER_H__

#include <string>
#include <glm/glm.hpp>

namespace Pionner
{
	class RhiShader
	{
	public:
		RhiShader() {};
		virtual ~RhiShader() = default;

		virtual bool initialize() = 0;
		virtual void activate(bool active) = 0;
		virtual bool shutdown() = 0;

		virtual bool isInit() const { return false; }
		virtual void setInt(const std::string &name, int val) const {}
		virtual void setFloat(const std::string &name, float val) const {}
		virtual void setMat3(const std::string &name, const glm::mat3 &val) const {}
		virtual void setMat4(const std::string &name, const glm::mat4 &val) const {}
		virtual void setVec2(const std::string &name, const glm::vec2 &val) const {}
		virtual void setVec3(const std::string &name, const glm::vec3 &val) const {}
		virtual void setVec4(const std::string &name, const glm::vec4 &val) const {}
	};
}

#endif