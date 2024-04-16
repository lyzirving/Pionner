#ifndef __PIONNER_CORE_MATH_INTERSECTION_H__
#define __PIONNER_CORE_MATH_INTERSECTION_H__

#include "Ray.h"
#include "OBB.h"

namespace pio
{
	bool Intersection(const Ray &ray, const OBB &obb, float distance);
}

#endif