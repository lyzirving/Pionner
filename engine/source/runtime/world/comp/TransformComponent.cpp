#include "TransformComponent.h"

namespace pio
{
	TransformComponent::TransformComponent() : Component()
	{
		m_define = CMP_TRANSFORM;
	}

	TransformComponent::~TransformComponent() = default;
}