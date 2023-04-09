#include "function/framework/components/transform/TransformComponent.h"
#include "function/framework/object/GameObject.h"

namespace Pionner
{
	TransformComponent::TransformComponent() : Component()
		                                     , m_transform()
											 , m_curIndex(0), m_nextIndex(1)
		                                     , m_dirty(false)
	{
		m_type = CMP_TRANSFORM;
	}

	TransformComponent::~TransformComponent() = default;

	void TransformComponent::postLoadResource(const std::weak_ptr<GameObject>& parent)
	{
		Transform default{};
		m_parent = parent;
		m_transform[m_curIndex]  = default;
		m_transform[m_nextIndex] = default;
	}

	void TransformComponent::tick(float delta)
	{
		std::swap(m_curIndex, m_nextIndex);
		if (m_dirty)
		{
			m_dirty = false;
			auto parent = m_parent.lock();
			if (parent)
			{
				// TODO: use current transform to do the tick
			}
		}
	}

	void TransformComponent::setPosition(const glm::vec3 pos)
	{
		if (m_transform[m_nextIndex].position(pos.x, pos.y, pos.z))
		{
			m_dirty = true;
		}
	}

	void TransformComponent::setScale(const glm::vec3 scale)
	{
		if (m_transform[m_nextIndex].scale(scale.x, scale.y, scale.z))
		{
			m_dirty = true;
		}
	}

	void TransformComponent::rotateByX(float angle, bool reverse)
	{
		if (m_transform[m_nextIndex].rotateByX(angle, reverse))
		{
			m_dirty = true;
		}
	}

	void TransformComponent::rotateByY(float angle, bool reverse)
	{
		if (m_transform[m_nextIndex].rotateByY(angle, reverse))
		{
			m_dirty = true;
		}
	}

	void TransformComponent::rotateByZ(float angle, bool reverse)
	{
		if (m_transform[m_nextIndex].rotateByZ(angle, reverse))
		{
			m_dirty = true;
		}
	}
}