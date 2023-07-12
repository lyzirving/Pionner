#ifndef __PIONNER_GFX_MATL_MATERIAL_H__
#define __PIONNER_GFX_MATL_MATERIAL_H__

#include <glm/glm.hpp>
#include <string>

namespace pio
{
	namespace gfx
	{
		class Material
		{
		public:
			Material();
			Material(const Material &rhs);
			virtual ~Material();

			Material &operator=(const Material &rhs);

			inline const glm::vec3 &getAmbient() { return m_ambientColor; }
			inline const glm::vec3 &getDifffuse() { return m_diffuseColor; }
			inline const glm::vec3 &getSpecular() { return m_specularColor; }
			inline const std::string &getName() { return m_name; }
			inline float getShiness() { return m_shiness; }

			inline void setAmbient(const glm::vec3 &color)  { m_ambientColor = color; }
			inline void setDiffuse(const glm::vec3 &color)  { m_diffuseColor = color; }
			inline void setSpecular(const glm::vec3 &color) { m_specularColor = color; }
			inline void setName(const std::string &name) { m_name = name; }
			inline void setShiness(float val) { m_shiness = val; }

		protected:
			// each channel for ambient, diffuse, specular ranges from [0, 1]
			glm::vec3   m_ambientColor{ 0.f };
			glm::vec3   m_diffuseColor{ 0.f };
			glm::vec3   m_specularColor{ 0.f };
			// shiness ranges from [0, 1000]
			float       m_shiness{ 0.f };
			std::string m_name;
		};
	}
}

#endif	