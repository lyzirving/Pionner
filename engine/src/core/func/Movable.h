#ifndef __PIONNER_CORE_FUNC_MOVABLE_H__
#define __PIONNER_CORE_FUNC_MOVABLE_H__

#include "core/math/Transform.h"

namespace pio
{
	class Movable
	{
	public:
		Movable() {}
		virtual ~Movable() = default;

		virtual void setTranslation(const glm::vec3 &val) { if (m_transform.Position != val) { m_transform.Position = val; invalidTransform(); } }
		virtual void setScale(const glm::vec3 &val) { if (m_transform.Scale != val) { m_transform.Scale = val; invalidTransform(); } }
		virtual void setRotation(const EulerAngle &val) { if (m_transform.Euler != val) { m_transform.Euler = val; invalidTransform(); } }

		virtual void setLocalTranslation(const glm::vec3 &val) { if (m_localTransform.Position != val) { m_localTransform.Position = val; invalidTransform(); } }
		virtual void setLocalScale(const glm::vec3 &val) { if (m_localTransform.Scale != val) { m_localTransform.Scale = val; invalidTransform(); } }
		virtual void setLocalRotation(const EulerAngle &val) { if (m_localTransform.Euler != val) { m_localTransform.Euler = val; invalidTransform(); } }

		virtual glm::mat4 getTransform() { return m_transform.mat(); };
		virtual glm::mat4 getLocalTransform() { return m_localTransform.mat(); };

		void invalidTransform(bool val = true) { m_bTransDirty = val; }
		bool bTransformChange() { return m_bTransDirty; }

	protected:
		bool m_bTransDirty{ true };
		Transform m_transform;
		Transform m_localTransform;
	};
}

#endif