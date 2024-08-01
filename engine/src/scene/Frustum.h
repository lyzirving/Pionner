#ifndef __PIONNER_SCENE_FRUSTUM_H__
#define __PIONNER_SCENE_FRUSTUM_H__

#include "core/Base.h"

namespace pio
{
	enum ProjectionType : uint8_t
	{
		ProjectionType_Perspective,
		ProjectionType_Orthographic,
		ProjectionType_Num
	};

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

	protected:
		ProjectionType m_type{ ProjectionType_Perspective };

		float m_aspect{ 1.f };
		float m_near{ 0.1f }, m_far{ 30.f };

		glm::mat4 m_mat{ 1.f };
		bool m_invalidate{ true };
	};
}

#endif