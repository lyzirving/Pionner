#ifndef __RENDER_SHADER_H__
#define __RENDER_SHADER_H__

#include <string>

#include <glm/glm.hpp>

namespace pio
{
	class Shader
	{
	public:
		Shader(const char *name, const char *vertName, const char *fragName);
		Shader(const char *name, const char *vertName, const char *fragName, const char *geoName);
		virtual ~Shader();

		inline bool isInit() { return m_program > 0; }
		inline const std::string &name() { return m_name; }
		inline const uint32_t getProgram() { return m_program; }

		bool init();
		void destroy();

		void use(bool active);

		void setFloat(const std::string &name, float value);
		void setInt(const std::string &name, int value);

		void setMat3(const std::string &name, const glm::mat3 &mat);
		void setMat4(const std::string &name, const glm::mat4 &mat);

		void setVec2(const std::string &name, const glm::vec2 &vec2);
		void setVec2(const std::string &name, float val0, float val1);
		void setVec3(const std::string &name, const glm::vec3 &vec3);
		void setVec3(const std::string &name, float val0, float val1, float val2);
		void setVec4(const std::string &name, const glm::vec4 &vec4);
		void setVec4(const std::string &name, float x0, float x1, float x2, float x3);

	protected:
		std::string m_name{};
		std::string m_vert{}, m_frag{}, m_geo{};
		uint32_t    m_program{ 0 };

	};
}

#endif