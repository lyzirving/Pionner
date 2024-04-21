#ifndef __PIONNER_CORE_FUNC_HITTABLE_HITTABLE_FLAT_CIRCLE_H__
#define __PIONNER_CORE_FUNC_HITTABLE_HITTABLE_FLAT_CIRCLE_H__

#include "HittableShape.h"
#include "core/math/Plane.h"

namespace pio
{
	class HittableFlatCircle : public HittableShape
	{
		OVERRIDE_HITTABLE_SHAPE(HitShapeType::FlatCircle)
	public:
		HittableFlatCircle(float radius);
		~HittableFlatCircle() = default;

		virtual void onCreateShape() override;
		virtual bool onHit(HitQuery &query) override;
		virtual void onDraw(const DrawParam &param) override;

	private:
		void resetData() { m_origin = glm::vec3(0.f); m_upDir = AXIS_Y; m_plane = Plane(AXIS_Z, m_origin); }
		void updateData();

	private:
		float m_innerRadius{0.f}, m_outerRaidus{ 0.f }, m_radius{ 0.f };
		glm::vec3 m_origin{ 0.f }, m_upDir{ AXIS_Y };
		Plane m_plane{ AXIS_Z, m_origin };
	};
}

#endif