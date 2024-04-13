#ifndef __PIONNER_CORE_HITTABLE_HITTABLE_H__
#define __PIONNER_CORE_HITTABLE_HITTABLE_H__

#include "HittableShape.h"

namespace pio
{
	struct HitResult
	{		
	};	

	class Hittable
	{
	public:
		Hittable() {}
		virtual ~Hittable() = default;
		virtual bool onHit(const HitResult result) = 0;
	};
}

#endif