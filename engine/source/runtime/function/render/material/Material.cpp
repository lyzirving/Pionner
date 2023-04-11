#include "function/render/material/Material.h"

namespace Pionner
{
	Material::Material() : m_path()
		                 , m_type(MAT_NONE)
		                 , m_id(0)
		                 , m_colorDiffuse(0.f), m_colorSpecular(0.f), m_colorAmbient(0.f)
		                 , m_shiness(1.f)
	{
	}

	Material::~Material()
	{
	}

}