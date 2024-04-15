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

		void setTranslation(const glm::vec3 &val) { m_transform.Position = val; }
		void setScale(const glm::vec3 &val) { m_transform.Scale = val; }
		void setRotation(const glm::vec3 &val) { m_transform.Euler = val; }
		void setRotation(const EulerAngle &val) { m_transform.Euler = val; }

		void setLocalTranslation(const glm::vec3 &val) { m_localTransform.Position = val; }
		void setLocalScale(const glm::vec3 &val) { m_localTransform.Scale = val; }
		void setLocalRotation(const glm::vec3 &val) { m_localTransform.Euler = val; }
		void setLocalRotation(const EulerAngle &val) { m_localTransform.Euler = val; }

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