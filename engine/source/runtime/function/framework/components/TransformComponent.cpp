#include "function/framework/components/TransformComponent.h"
#include "function/framework/object/GameObject.h"

namespace Pionner
{
	TransformComponent::TransformComponent() : Component()
	{
	}

	TransformComponent::~TransformComponent() = default;

	void TransformComponent::postLoadResource(const std::weak_ptr<GameObject>& parent)
	{
	}

	void TransformComponent::tick(float delta)
	{
	}
}