#ifndef __PIONNER_CORE_HITTABLE_HITTABLE_SHAPE_H__
#define __PIONNER_CORE_HITTABLE_HITTABLE_SHAPE_H__

#include "core/Base.h"

namespace pio
{
	enum class HitShapeType : uint8_t
	{
		None
	};

	class HittableShape
	{
	public:
		HittableShape() {}
		virtual ~HittableShape() = default;

		void setLocalTransform(const glm::mat4 &val)  { m_localTransform = val; }
		void setGlobalTransform(const glm::mat4 &val) { m_globalTransform = val; }

	protected:
		HitShapeType m_type{ HitShapeType::None };
		glm::mat4 m_localTransform{ 1.f }, m_globalTransform{ 1.f };
	};
}

#endif