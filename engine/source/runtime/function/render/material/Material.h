#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#include <string>
#include <glm/glm.hpp>

#include "function/render/material/MaterialDef.h"

namespace Pionner
{
	class Material
	{
	public:
		Material();
		~Material();

	public:
		MaterialType m_type;
		uint32_t     m_slot;

		glm::vec3    m_colorDiffuse;
		glm::vec3    m_colorSpecular;
		glm::vec3    m_colorAmbient;
		float        m_shiness;
	};
}

#endif