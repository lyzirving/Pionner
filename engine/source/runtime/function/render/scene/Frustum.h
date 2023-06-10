#ifndef __RENDER_FRUSTUM_H__
#define __RENDER_FRUSTUM_H__

#include <atomic>
#include <glm/glm.hpp>
#include <list>

namespace Pionner
{
	class Frustum
	{
	public:
		Frustum(float fov = 60.f, float aspect = 1.f, float near = 0.1f, float far = 30.f);
		~Frustum();

		const glm::mat4 &getPerspectMat();
		const glm::mat4 &getPerspectInvMat();
		const glm::mat4 &getOrthoMat();

		void setFov(float fov);
		void setAspect(float aspect);
		void setNearFar(float near, float far);
		void setOrtho(float l, float r, float b, float t);

		void restoreState();
		void popState();

		inline float near() { return m_near; }
		inline float far() { return m_far; }
		inline float fov() { return m_fov; }
		inline float aspect() { return m_aspect; };
		inline bool  isChanged() { return m_change.load(); }

	private:
		struct FrustumState
		{
			float m_fov, m_aspect;
			float m_near, m_far;

			FrustumState() : m_fov(0.f), m_aspect(0.f), m_near(0.f), m_far(0.f)
			{}

			FrustumState(float fov, float aspect, float near, float far)
				: m_fov(fov), m_aspect(aspect), m_near(near), m_far(far)
			{}
		};

	private:
		void calcProjectMat();

	private:

		float m_fov, m_aspect;
		float m_near, m_far;
		std::atomic_bool m_change;
		std::list<FrustumState> m_stateStack;

		glm::mat4 m_perspectMat;
		glm::mat4 m_perspectInvMat;

		float m_orthoLeft, m_orthoRight;
		float m_orthoBottom, m_orthoTop;
		glm::mat4 m_orthoMat;
	};
}

#endif