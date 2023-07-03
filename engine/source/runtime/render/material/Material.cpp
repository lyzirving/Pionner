#include "render/material/Material.h"

namespace pio
{
	Material::Material()
		: m_ambientSlot(-1), m_diffSlot(-1), m_specSlot(-1)
		, m_normalSlot(-1)
		, m_name()
		, m_colorAmbient(0.f)
		, m_colorDiffuse(0.f)
		, m_colorSpecular(0.f)
		, m_shiness(1.f)
		, m_mode(MODE_CONSTANT)
	{
	}

	Material::~Material()
	{

	}
}