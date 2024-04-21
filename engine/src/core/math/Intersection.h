#ifndef __PIONNER_CORE_MATH_INTERSECTION_H__
#define __PIONNER_CORE_MATH_INTERSECTION_H__

#include "core/Base.h"

namespace pio
{
	class  AABB;
	class  Plane;
	struct HitQuery;

	bool Intersection(HitQuery &query/*in and out*/, const AABB &aabb);
	bool Intersection(HitQuery &query/*in and out*/, const Plane &plane);
}

#endif