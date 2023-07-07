#include "Material.h"

namespace pio
{
	namespace matl
	{
		Material::Material()
		{
		}

		Material::Material(const Material &rhs) 
			: m_ambientColor(rhs.m_ambientColor)
			, m_diffuseColor(rhs.m_diffuseColor)
			, m_specularColor(rhs.m_specularColor)
			, m_shiness(rhs.m_shiness)
			, m_name(rhs.m_name)
		{
		}

		Material::~Material() = default;

		Material &Material::operator=(const Material &rhs)
		{
			if (this != &rhs)
			{
				m_ambientColor = rhs.m_ambientColor;
				m_specularColor = rhs.m_specularColor;
				m_diffuseColor = rhs.m_diffuseColor;
				m_shiness = rhs.m_shiness;
				m_name = rhs.m_name;
			}
			return *this;
		}
	}
}