#ifndef __PIONNER_CORE_FUNC_HITTABLE_HITTABLE_H__
#define __PIONNER_CORE_FUNC_HITTABLE_HITTABLE_H__

#include "core/math/Ray.h"

namespace pio
{
	struct HitQuery
	{	
		Ray R{};
		glm::vec3 HitPt{ 0.f };
		void *HitActor{ nullptr };//TODO: add interface for hit actor?
		bool Hit{ false };

		HitQuery() {}
		HitQuery(const Ray &r) : R(r) {}
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