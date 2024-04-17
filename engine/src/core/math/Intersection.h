#ifndef __PIONNER_CORE_MATH_INTERSECTION_H__
#define __PIONNER_CORE_MATH_INTERSECTION_H__

#include "AABB.h"

namespace pio
{
	bool Intersection(HitQuery &query/*in and out*/, const AABB &aabb);
}

#endif