#ifndef __PIONNER_CORE_INTERFACE_HITTABLE_HITTABLE_BOX_H__
#define __PIONNER_CORE_INTERFACE_HITTABLE_HITTABLE_BOX_H__

#include "HittableShape.h"

#include "core/math/AABB.h"

namespace pio
{
	/*
	* @brief: HittableBox is specified by an origin and 3d length, and its origin is always located at (0.f, 0.f, 0.f)
	*/
	class HittableBox : public HittableShape
	{
	OVERRIDE_HITTABLE_SHAPE(HitShapeType::Box)

	public:
		HittableBox();
		HittableBox(const glm::vec3 &len);
		~HittableBox() = default;

		virtual void onCreateShape() override;
		virtual bool onHit(HitQuery &query) override;
		virtual void onDraw(const DrawParam &param) override;

	private:
		glm::vec3 m_len{ 1.f };
		AABB m_aabb{};
	};
}

#endif