#ifndef __PIONNER_CORE_INTERFACE_HITTABLE_HITTABLE_H__
#define __PIONNER_CORE_INTERFACE_HITTABLE_HITTABLE_H__

#include "core/math/Ray.h"

namespace pio
{
	struct HitQuery
	{	
		Ray  R{};
		glm::vec3 Intersection{ 0.f };
		bool Hit{ false };
	};	

	class Hittable
	{
	public:
		Hittable() {}
		virtual ~Hittable() = default;
		virtual bool onHit(HitQuery &query) = 0;
	};
}

#endif