#ifndef __PIONNER_CORE_INTERFACE_MOVABLE_H__
#define __PIONNER_CORE_INTERFACE_MOVABLE_H__

#include "core/math/Transform.h"

namespace pio
{
	class Movable
	{
	public:
		Movable() {}
		~Movable() = default;

		virtual void setTranslation(const glm::vec3 &val) { m_transform.Position = val; }
		virtual void setScale(const glm::vec3 &val) { m_transform.Scale = val; }
		virtual void setRotation(const glm::vec3 &val) { m_transform.Euler = val; }
		virtual void setRotation(const EulerAngle &val) { m_transform.Euler = val; }

		virtual void setLocalTranslation(const glm::vec3 &val) { m_localTransform.Position = val; }
		virtual void setLocalScale(const glm::vec3 &val) { m_localTransform.Scale = val; }
		virtual void setLocalRotation(const glm::vec3 &val) { m_localTransform.Euler = val; }
		virtual void setLocalRotation(const EulerAngle &val) { m_localTransform.Euler = val; }

		glm::mat4 getTransform() { return m_transform.mat(); }
		glm::mat4 getTransform() const { return m_transform.mat(); }
		glm::mat4 getLocalTransform() { return m_localTransform.mat(); }
		glm::mat4 getLocalTransform() const { return m_localTransform.mat(); }

	protected:		
		Transform m_transform;
		Transform m_localTransform;
	};
}

#endif