#ifndef __PIONNER_SCENE_PERSPECTIVE_FRUSTUM_H__
#define __PIONNER_SCENE_PERSPECTIVE_FRUSTUM_H__

#include "Frustum.h"

namespace pio
{
	class PerspectiveFrustum : public Frustum
	{
	public:
		PerspectiveFrustum() : Frustum(ProjectionType_Perspective) {}
		~PerspectiveFrustum() = default;

		PerspectiveFrustum(const PerspectiveFrustum &rhs);
		PerspectiveFrustum &operator=(const PerspectiveFrustum &rhs);

		void setFov(float fov);

	public:
		float fov() const { return m_fov; }

		virtual float top()    const override { return m_near * std::tan(glm::radians(m_fov * 0.5f)); }
		virtual float bottom() const override { return -top(); }
		virtual float right()  const override { return top() * m_aspect; }
		virtual float left()   const override { return -right(); }

	protected:
		virtual void calcMat() override;

	private:
		float m_fov{ 60.f };
	};
}

#endif