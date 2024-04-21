#include "HittableFlatCircle.h"

#include "core/math/Intersection.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "HittableFlatCircle"

#define TOUCH_THREASHOLD (0.04f)

namespace pio
{
	HittableFlatCircle::HittableFlatCircle(float radius) : HittableShape(), m_radius(radius)
	{
		m_innerRadius = radius - TOUCH_THREASHOLD;
		m_outerRaidus = radius + TOUCH_THREASHOLD;
		onCreateShape();
	}

	void HittableFlatCircle::onCreateShape()
	{
	}

	bool HittableFlatCircle::onHit(HitQuery &query)
	{
		updateData();
		Intersection(query, m_plane);
		if (query.Hit)
		{
			float distance = glm::distance(query.HitPt, m_origin);
			float dotVal   = glm::dot(m_upDir, query.HitPt - m_origin);
			query.Hit = distance >= m_innerRadius && distance <= m_outerRaidus && dotVal >= 0.f;
			/*LOGD("ray hit plane, final result[%s], distance[%f], radius[%f], dot[%f]", 
				 query.Hit ? "true" : "false", distance, m_radius, dotVal);*/
		}
		return query.Hit;
	}

	void HittableFlatCircle::onDraw(const DrawParam &param)
	{
	}

	void HittableFlatCircle::updateData()
	{
		if (!bTransformChange())
			return;

		invalidTransform(false);

		resetData();

		glm::vec3 localTrans = m_localTransform.Position;
		glm::mat4 localRot = m_localTransform.Euler.mat();

		glm::vec3 trans = m_transform.Position;
		glm::mat4 rot = m_transform.Euler.mat();

		glm::mat4 transMat, localMat;
		localMat = m_localTransform.mat();
		transMat = m_transform.mat();

		m_origin = transMat * localMat * glm::vec4(m_origin, 1.f);
		m_upDir  = transMat * localMat * glm::vec4(m_upDir, 0.f);

		auto calcPlane = [](const glm::vec3 &_translation, const glm::mat4 &_rotMat, Plane &_plane)
		{
			const glm::vec4 abcd = glm::vec4(_plane.getNormal(), _plane.getD());
			// rotation mat is Orthogonal matrix, so its interse equals transpose.
			glm::mat4 invMat = glm::transpose(_rotMat);
			glm::vec3 N; float D;
			N.x = glm::dot(abcd, invMat[0]);
			N.y = glm::dot(abcd, invMat[1]);
			N.z = glm::dot(abcd, invMat[2]);
			N = glm::normalize(N);

			D = glm::dot(abcd, invMat[3]);
			D += -glm::dot(N, _translation);

			_plane = glm::vec4(N, D);
		};

		calcPlane(localTrans, localRot, m_plane);
		calcPlane(trans, rot, m_plane);
	}
}