#ifndef __PIONNER_SCENE_3D_FRUSTUM_H__
#define __PIONNER_SCENE_3D_FRUSTUM_H__

#include "Common.h"

namespace pio
{
	class Frustum
	{
	public:
		Frustum() {}
		Frustum(ProjectionType type) : m_type(type) {}
		virtual ~Frustum() = default;

		Frustum(const Frustum &rhs);
		Frustum &operator=(const Frustum &rhs);

	public:
		void flush();
		void setAspect(float aspect);
		void setNear(float near);
		void setFar(float far);

		void invalidate(bool invalidate = true) { m_invalidate = invalidate; }
		bool bDirty() const { return m_invalidate; }
		ProjectionType type() const { return m_type; }

	public:
		virtual float top()    const = 0;
		virtual float bottom() const = 0;
		virtual float right()  const = 0;
		virtual float left()   const = 0;

	public:
		float aspect() const { return m_aspect; };
		float near()   const { return m_near; }
		float far()    const { return m_far; }

		const glm::mat4& mat() const { return m_mat; }

	protected:
		virtual void calcMat() = 0;

	public:
		static glm::mat4 PerspectiveMat(float fov, float aspect, float near, float far);
		static glm::mat4 OrthoMat(float left, float right, float bottom, float top, float near, float far);

	protected:
		ProjectionType m_type{ ProjectionType_Perspective };

		float m_aspect{ 1.f };
		float m_near{ 0.1f }, m_far{ 30.f };

		glm::mat4 m_mat{ 1.f };
		bool m_invalidate{ true };
	};

	class PerspectiveFrustum : public Frustum
	{
	public:
		PerspectiveFrustum() : Frustum(ProjectionType_Perspective) {}
		~PerspectiveFrustum() = default;

		PerspectiveFrustum(const PerspectiveFrustum& rhs);
		PerspectiveFrustum& operator=(const PerspectiveFrustum& rhs);

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

	class OrthographicFrustum : public Frustum
	{
	public:
		OrthographicFrustum() : Frustum(ProjectionType_Orthographic) {}
		~OrthographicFrustum() = default;

		OrthographicFrustum(const OrthographicFrustum& rhs);
		OrthographicFrustum& operator=(const OrthographicFrustum& rhs);

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