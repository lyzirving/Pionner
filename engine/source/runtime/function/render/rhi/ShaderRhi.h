#ifndef __RHI_SHADER_H__
#define __RHI_SHADER_H__

#include <string>
#include <glm/glm.hpp>

namespace Pionner
{
	class ShaderRhi
	{
	public:
		ShaderRhi() {};
		virtual ~ShaderRhi() = default;

		virtual bool build(const char *vert, const char *frag, uint32_t &program) = 0;
		virtual void activate(uint32_t program, bool active) = 0;
		virtual void destroy(uint32_t &program) = 0;

		virtual bool getAttribLoc(uint32_t program, const std::string &name, int &loc) const { return false; }

		virtual void setInt(uint32_t program, const std::string &name, int val) const {}
		virtual void setFloat(uint32_t program, const std::string &name, float val) const {}
		virtual void setMat3(uint32_t program, const std::string &name, const glm::mat3 &val) const {}
		virtual void setMat4(uint32_t program, const std::string &name, const glm::mat4 &val) const {}
		virtual void setVec2(uint32_t program, const std::string &name, const glm::vec2 &val) const {}
		virtual void setVec2(uint32_t program, const std::string &name, float val0, float val1) const {}
		virtual void setVec3(uint32_t program, const std::string &name, const glm::vec3 &val) const {}
		virtual void setVec3(uint32_t program, const std::string &name, float val0, float val1, float val2) const {}
		virtual void setVec4(uint32_t program, const std::string &name, const glm::vec4 &val) const {}
		virtual void setVec4(uint32_t program, const std::string &name, float val0, float val1, float val2, float val3) const {}
	};
}

#endif