#ifndef STB_IMAGE_STATIC
#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#endif // !STB_IMAGE_STATIC
#include <stb/stb_image.h>

#include "function/render/material/Material.h"

namespace Pionner
{
	Material::Material() : m_path()
		                 , m_imageRaw()
		                 , m_type(MAT_NONE)
		                 , m_id(0)
		                 , m_colorDiffuse(0.f), m_colorSpecular(0.f), m_colorAmbient(0.f)
		                 , m_shiness(1.f)
	{
	}

	Material::~Material()
	{
		releaseRawData();
	}

	void Material::loadRawData()
	{
		releaseRawData();
		if (!m_path.empty())
		{
			uint8_t *data = stbi_load(m_path.c_str(), 
				                      &m_imageRaw.m_width, &m_imageRaw.m_height,
				                      &m_imageRaw.m_channel, 0);
			if (data)
			{
				m_imageRaw.m_data = data;
			}
		}
	}

	void Material::releaseRawData()
	{
		if (m_imageRaw.m_data)
		{
			stbi_image_free(m_imageRaw.m_data);
			m_imageRaw.m_data = nullptr;
			m_imageRaw.m_width = 0;
			m_imageRaw.m_height = 0;
			m_imageRaw.m_channel = 0;
		}
	}
}