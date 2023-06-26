#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#include <string>
#include <memory>
#include <glm/glm.hpp>

namespace Pionner
{
	enum ShadingMode : uint8_t
	{
		MODE_NONE,
		MODE_CONSTANT, // Constant model, color = kd. In mtl file, illum = 0.
		MODE_DIFFUSE, // Diffuse model, color = ambient + diffuse. In mtl file, illum = 1.
		MODE_DIFFUSE_SPECULAR, // Mixed model, color = ambient + diffuse + specular. In mtl file, illum = 2.
		MODE_COUNT
	};

	class Material
	{
	public:
		Material();
		~Material();

		bool ambientValid() const { return m_ambientSlot >= 0; }
		bool diffValid() const { return m_diffSlot >= 0; }
		bool specValid() const { return m_specSlot >= 0; }

	public:
		int32_t      m_ambientSlot;
		int32_t      m_diffSlot;
		int32_t      m_specSlot;

		std::string  m_name;

		// each channel for ambient, diffuse, specular ranges from [0, 1]
		glm::vec3    m_colorAmbient;
		glm::vec3    m_colorDiffuse;
		glm::vec3    m_colorSpecular;

		// shiness ranges from [0, 1000]
		float        m_shiness;

		ShadingMode  m_mode;
	};
}

#endif