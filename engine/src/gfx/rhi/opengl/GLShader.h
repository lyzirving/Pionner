#ifndef __PIONNER_GFX_RHI_OPENGL_GL_SHADER_H__
#define __PIONNER_GFX_RHI_OPENGL_GL_SHADER_H__

#include "gfx/rhi/Shader.h"

namespace pio
{
	class GLShader : public Shader
	{
	public:
		GLShader(Ref<RenderContext>& context);
		~GLShader() = default;

		virtual void setFloat(const std::string &name, float val) override;
		virtual void setUInt(const std::string &name, uint32_t val) override;
		virtual void setBool(const std::string &name, bool val) override;
		virtual void setInt(const std::string &name, int32_t val) override;
		virtual void setTextureSampler(const std::string &name, TextureSampler slot) override;

		virtual void setMat3(const std::string &name, const glm::mat3 &mat) override;
		virtual void setMat4(const std::string &name, const glm::mat4 &mat) override;

		virtual void setVec2(const std::string &name, const glm::vec2 &vec2) override;
		virtual void setVec3(const std::string &name, const glm::vec3 &vec3) override;
		virtual void setVec4(const std::string &name, const glm::vec4 &vec4) override;

		virtual void setIVec2(const std::string &name, const glm::ivec2 &vec2) override;
		virtual void setIVec3(const std::string &name, const glm::ivec3 &vec3) override;
		virtual void setIVec4(const std::string &name, const glm::ivec4 &vec4) override;

		virtual bool init() override;
		virtual bool isInit() const override { return m_id > 0; }
		virtual void destroy() override;

		virtual void bind() override;
		virtual void unbind() override;
		virtual const std::string& name() const override { return m_name; };
		virtual bool getSampler(TextureSampler& out) const override;

	private:
		std::string m_name{};
		std::string m_vert{}, m_frag{}, m_geo{};
		std::bitset<PIO_UINT8(TextureSampler::MaxSlotNum)> m_occupiedSlots;

	private:
		friend class GLShaderCompiler;
	};
}

#endif