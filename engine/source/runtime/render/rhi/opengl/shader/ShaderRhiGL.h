#ifndef __RHI_SHADER_GL_H__
#define __RHI_SHADER_GL_H__

#include "render/rhi/ShaderRhi.h"

namespace pio
{
	class ShaderRhiGL : public ShaderRhi
	{
	public:
		ShaderRhiGL();
		virtual ~ShaderRhiGL();

		virtual bool build(const char *vert, const char *frag, uint32_t &program) override;
		virtual bool build(const char *vert, const char *frag, const char *geo, uint32_t &program) override;
		virtual void activate(uint32_t program, bool active) override;
		virtual void destroy(uint32_t &program) override;

		virtual void setInt(uint32_t program, const std::string &name, int val) const override;
		virtual void setFloat(uint32_t program, const std::string &name, float val) const override;
		virtual void setMat3(uint32_t program, const std::string &name, const glm::mat3 &val) const override;
		virtual void setMat4(uint32_t program, const std::string &name, const glm::mat4 &val) const override;
		virtual void setVec2(uint32_t program, const std::string &name, const glm::vec2 &val) const override;
		virtual void setVec2(uint32_t program, const std::string &name, float val0, float val1) const override;
		virtual void setVec3(uint32_t program, const std::string &name, const glm::vec3 &val) const override;
		virtual void setVec3(uint32_t program, const std::string &name, float val0, float val1, float val2) const override;
		virtual void setVec4(uint32_t program, const std::string &name, const glm::vec4 &val) const override;
		virtual void setVec4(uint32_t program, const std::string &name, float val0, float val1, float val2, float val3) const override;

	private:
		inline bool isInit(uint32_t program) const { return program > 0; }

	};
}

#endif