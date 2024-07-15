#ifndef __PIONNER_GFX_STRUCT_CAMERA_2_H__
#define __PIONNER_GFX_STRUCT_CAMERA_2_H__

#include "Camera.h"
#include "core/math/Transform.h"

namespace pio
{
	struct CameraPose
	{
		glm::vec3 LookAt{ 0.f };
		// camera's +x/+y/+z axis
		glm::vec3 Right{ 1.f, 0.f, 0.f }, Up{ 0.f, 1.f, 0.f }, Front{ 0.f, 0.f, 1.f };
		glm::mat4 ViewMat{ 1.f };
	};

	class Camera2
	{
	public:
		Camera2() {}
		~Camera2() = default;

		void flush();

		void setPosition(const glm::vec3& position);
		void setPosition(const SphereCoord &position);
		void setLookAt(const glm::vec3& lookAt);

		void setFov(float fov) { m_frustum.setFov(fov); }
		void setAspect(float aspect) { m_frustum.setAspect(aspect); }
		void setNearFar(float near, float far) { m_frustum.setNearFar(near, far); }
		void setOrtho(float l, float r, float b, float t) { m_frustum.setOrtho(l, r, b, t); }
		void setViewport(int32_t x, int32_t y, int32_t w, int32_t h) { m_viewport = Viewport{ x, y, w, h }; }

		float near() const { return m_frustum.near(); }
		float far() const { return m_frustum.far(); }
		float fov() const { return m_frustum.fov(); }

		float top() const { return m_frustum.top(); }
		float bottom() const { return m_frustum.bottom(); }
		float right() const { return m_frustum.right(); }
		float left() const { return m_frustum.left(); }

		const glm::mat4& getViewMat() const { return m_pose.ViewMat; }
		const glm::mat4& getPrjMat() const { return m_frustum.getPerspectMat(); }
		const glm::mat4& getOrthoMat() const { return m_frustum.getOrthoMat(); }
		const Viewport& getViewport() const { return m_viewport; }

		const glm::vec3& getRight() const { return m_pose.Right; }
		const glm::vec3& getUp()    const { return m_pose.Up; }
		const glm::vec3& getFront() const { return m_pose.Front; }

	private:
		void calcViewMat();
		void calcCameraPose();

	private:
		Frustum m_frustum{};
		Viewport m_viewport{};
		Transform m_transform{};
		CameraPose m_pose{};
		bool m_dirty{ true };
	};
}

#endif