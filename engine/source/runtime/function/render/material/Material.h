#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#include <string>
#include <memory>
#include <glm/glm.hpp>

#include "function/render/material/MaterialDef.h"

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

		bool slotValid() const { return m_slot >= 0; }

	public:
		MaterialType m_type;
		int32_t      m_slot;

		std::string  m_name;

		// each channel for ambient, diffuse, specular ranges from [0, 1]
		glm::vec3    m_colorAmbient;
		glm::vec3    m_colorDiffuse;
		glm::vec3    m_colorSpecular;

		// shiness ranges from [0, 1000]
		float        m_shiness;

		ShadingMode  m_mode;
		bool         m_hasTexture;
	};
}

#endif