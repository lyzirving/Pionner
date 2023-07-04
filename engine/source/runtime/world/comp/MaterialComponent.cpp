#include "MaterialComponent.h"

namespace pio
{
	MaterialComponent::MaterialComponent() : Component()
	{
		m_define = CMP_MATERIAL;
	}

	MaterialComponent::~MaterialComponent() = default;
}