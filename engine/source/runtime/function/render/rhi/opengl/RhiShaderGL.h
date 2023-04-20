#ifndef __RHI_SHADER_GL_H__
#define __RHI_SHADER_GL_H__

#include "function/render/rhi/RhiShader.h"

namespace Pionner
{
	class RhiShaderGL : public RhiShader
	{
	public:
		RhiShaderGL();
		virtual ~RhiShaderGL();

		virtual bool initialize() override;
		virtual void activate(bool active) override;
		virtual bool shutdown() override;

		virtual bool isInit() const override;
		virtual void setInt(const std::string &name, int val) const override;
		virtual void setFloat(const std::string &name, float val) const override;
		virtual void setMat3(const std::string &name, const glm::mat3 &val) const override;
		virtual void setMat4(const std::string &name, const glm::mat4 &val) const override;
		virtual void setVec2(const std::string &name, const glm::vec2 &val) const override;
		virtual void setVec3(const std::string &name, const glm::vec3 &val) const override;
		virtual void setVec4(const std::string &name, const glm::vec4 &val) const override;

	};
}

#endif