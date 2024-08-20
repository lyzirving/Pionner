#ifndef __PIONNER_SCENE_ORTHOGRAPHIC_FRUSTUM_H__
#define __PIONNER_SCENE_ORTHOGRAPHIC_FRUSTUM_H__

#include "Frustum.h"

namespace pio
{
	class OrthographicFrustum : public Frustum
	{
	public:
		OrthographicFrustum() : Frustum(ProjectionType_Orthographic) {}
		~OrthographicFrustum() = default;

		OrthographicFrustum(const OrthographicFrustum &rhs);
		OrthographicFrustum &operator=(const OrthographicFrustum &rhs);

		void setSize(float size);

	public:
		float size() const { return m_size; }

		virtual float top()    const override { return m_size; }
		virtual float bottom() const override { return -top(); }
		virtual float right()  const override { return top() * m_aspect; }
		virtual float left()   const override { return -right(); }

	protected:
		virtual void calcMat() override;

	private:
		float m_size{ 1.f };
	};
}

#endif