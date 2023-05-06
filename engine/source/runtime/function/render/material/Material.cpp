#include "function/render/material/Material.h"

namespace Pionner
{
	Material::Material()
		: m_type(MAT_NONE)
		, m_slot(-1)
		, m_colorDiffuse(0.f)
		, m_colorSpecular(0.f)
		, m_colorAmbient(0.f)
		, m_shiness(1.f)
	{
	}

	Material::~Material()
	{

	}
}