#ifndef __PIONNER_PLATFORM_OPENGL_GL_SHADER_H__
#define __PIONNER_PLATFORM_OPENGL_GL_SHADER_H__

#include "gfx/rhi/Shader.h"

namespace pio
{
	class GLShader : public Shader
	{
	public:
		GLShader(const char *name, const char *vert, const char *frag);
		GLShader(const char *name, const char *vert, const char *frag, const char *geo);
		GLShader(const char* path);
		virtual ~GLShader();

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
		virtual void bind(bool active) override;
		virtual void destroy() override;
		virtual bool getAvailableSampler(TextureSampler &out) const override;

		virtual uint32_t getProgram() const override { return m_program; };
		virtual const std::string &getName() const override { return m_name; };
		virtual const std::string &getPath() const override { return m_path; };
		virtual bool isInit() const override { return m_program > 0; };

	private:
		std::string m_path{};
		std::string m_name{};
		std::string m_vert{}, m_frag{}, m_geo{};
		uint32_t m_program{ 0 };
		std::bitset<PIO_UINT8(TextureSampler::MaxSlotNum)> m_occupiedSlots;
	};
}

#endif