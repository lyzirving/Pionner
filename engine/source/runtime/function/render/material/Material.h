#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#include <string>
#include <glm/glm.hpp>

#include "function/render/material/MaterialDef.h"

namespace Pionner
{
	struct ImageRawData
	{
		uint8_t* m_data;
		int      m_width, m_height;
		int      m_channel;

		ImageRawData() : m_data(nullptr)
			           , m_width(0), m_height(0)
			           , m_channel(0)
		{
		}
	};

	class Material
	{
	public:
		Material();
		~Material();

		void loadRawData();
		void releaseRawData();

	public:
		std::string  m_path;
		ImageRawData m_imageRaw;
		MaterialType m_type;
		uint32_t     m_id;

		glm::vec3    m_colorDiffuse;
		glm::vec3    m_colorSpecular;
		glm::vec3    m_colorAmbient;
		float        m_shiness;
	};
}

#endif