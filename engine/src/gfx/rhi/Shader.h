#ifndef __PIONNER_GFX_RHI_SHADER_H__
#define __PIONNER_GFX_RHI_SHADER_H__

#include "RenderResource.h"

namespace pio
{
	class UniformData;

	class Shader : public RenderResource
	{
	public:
		Shader(Ref<RenderContext>& context) : RenderResource(context, RenderResourceType::Shader) {}
		virtual ~Shader() = default;

		virtual const std::string& name() const = 0;
		virtual bool getSampler(TextureSampler& out) const = 0;

		virtual void setBool(const std::string& name, bool val) = 0;
		virtual void setFloat(const std::string& name, float val) = 0;
		virtual void setInt(const std::string& name, int32_t val) = 0;
		virtual void setUInt(const std::string& name, uint32_t val) = 0;

		virtual void setMat2(const std::string& name, const glm::mat2& mat) = 0;
		virtual void setMat3(const std::string& name, const glm::mat3& mat) = 0;
		virtual void setMat4(const std::string& name, const glm::mat4& mat) = 0;

		virtual void setVec2(const std::string& name, const glm::vec2& vec2) = 0;
		virtual void setVec3(const std::string& name, const glm::vec3& vec3) = 0;
		virtual void setVec4(const std::string& name, const glm::vec4& vec4) = 0;

		virtual void setIVec2(const std::string& name, const glm::ivec2& vec2) = 0;
		virtual void setIVec3(const std::string& name, const glm::ivec3& vec3) = 0;
		virtual void setIVec4(const std::string& name, const glm::ivec4& vec4) = 0;

		virtual void setTextureSampler(const std::string& name, TextureSampler slot) = 0;

	public:
		void setUniformData(Ref<UniformData>& unimData);
	};
}

#endif