#ifndef __PIONNER_CORE_MATH_INTERSECTION_H__
#define __PIONNER_CORE_MATH_INTERSECTION_H__

#include "Ray.h"
#include "AABB.h"

namespace pio
{
	bool Intersection(const Ray &ray, const AABB &aabb, float distance);
}

#endif