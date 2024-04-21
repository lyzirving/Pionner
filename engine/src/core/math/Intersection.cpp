#include "Intersection.h"

#include "core/math/AABB.h"
#include "core/math/Plane.h"
#include "core/func/hittable/Hittable.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Intersection"

namespace pio
{
	bool Intersection(HitQuery &query, const AABB &aabb)
	{
		float t{ -1.f };
		const Ray &ray = query.R;
		glm::vec3 ptOnPlane{ 0.f };

		if (!Math::IsZero(ray.Dir.x))
		{
			//ray: P=O+D*t
			if (ray.Dir.x > 0)
				t = (aabb.Min.x - ray.Origin.x) / ray.Dir.x;
			else
				t = (aabb.Max.x - ray.Origin.x) / ray.Dir.x;

			if (t > 0.f)
			{
				ptOnPlane = ray.Origin + t * ray.Dir;
				if (aabb.Min.y < ptOnPlane.y && ptOnPlane.y < aabb.Max.y &&
					aabb.Min.z < ptOnPlane.z && ptOnPlane.z < aabb.Max.z)
				{
					query.Hit = true;
					query.HitPt = ptOnPlane;
					return true;
				}
			}
		}

		if (!Math::IsZero(ray.Dir.y))
		{
			if (ray.Dir.y > 0)
				t = (aabb.Min.y - ray.Origin.y) / ray.Dir.y;
			else
				t = (aabb.Max.y - ray.Origin.y) / ray.Dir.y;

			if (t > 0.f)
			{
				ptOnPlane = ray.Origin + t * ray.Dir;
				if (aabb.Min.z < ptOnPlane.z && ptOnPlane.z < aabb.Max.z &&
					aabb.Min.x < ptOnPlane.x && ptOnPlane.x < aabb.Max.x)
				{
					query.Hit = true;
					query.HitPt = ptOnPlane;
					return true;
				}
			}
		}

		if (!Math::IsZero(ray.Dir.z))
		{
			if (ray.Dir.z > 0)
				t = (aabb.Min.z - ray.Origin.z) / ray.Dir.z;
			else
				t = (aabb.Max.z - ray.Origin.z) / ray.Dir.z;

			if (t > 0.f)
			{
				ptOnPlane = ray.Origin + t * ray.Dir;
				if (aabb.Min.x < ptOnPlane.x && ptOnPlane.x < aabb.Max.x &&
					aabb.Min.y < ptOnPlane.y && ptOnPlane.y < aabb.Max.y)
				{
					query.Hit = true;
					query.HitPt = ptOnPlane;
					return true;
				}
			}
		}
		return false;
	}

	bool Intersection(HitQuery &query, const Plane &plane)
	{
		const glm::vec3 &p0 = query.R.Origin;
		const glm::vec3 &n0 = query.R.Dir;
		const glm::vec3 &n1 = plane.getNormal();
		float d = plane.getD();

		if (plane.bOnPlane(p0))
		{
			query.Hit = true;
			query.HitPt = p0;
			return true;
		}

		float nDot = glm::dot(n0, n1);
		// plane and ray are in parallel direction
		if (Math::Equal(nDot, 0.f))
			return false;

		float t = -(glm::dot(p0, n1) + d) / nDot;
		if (t >= 0.f)
		{
			query.Hit = true;
			query.HitPt = query.R.at(t);
			return true;
		}
		return false;
	}
}