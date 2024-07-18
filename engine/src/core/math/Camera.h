#ifndef __PIONNER_GFX_STRUCT_CAMERA_2_H__
#define __PIONNER_GFX_STRUCT_CAMERA_2_H__

#include "Frustum.h"

#include "gfx/renderer/RenderState.h"
#include "gfx/rhi/Uniform.h"
#include "core/math/Transform.h"

namespace pio
{
	enum class ProjectionType { None = 0, Perspective, Orthographic };

	struct CameraUD
	{
		PIO_UNIFORM_STRUCT_DECLARE(CameraUD)

		glm::mat4 ViewMat{ 1.f };
		glm::mat4 PrjMat{ 1.f };
		glm::mat4 OrthoMat{ 1.f };
		glm::vec3 CameraPosition{ 0.f };
		float FrustumFar{ 0.f };
	};

	struct CameraPose
	{
		glm::vec3 LookAt{ 0.f };
		// camera's +x/+y/+z axis
		glm::vec3 Right{ 1.f, 0.f, 0.f }, Up{ 0.f, 1.f, 0.f }, Front{ 0.f, 0.f, 1.f };
		glm::mat4 ViewMat{ 1.f };
	};

	enum CameraAttrBits : uint8_t
	{
		CameraAttrBits_Pos = 0, 
		CameraAttrBits_Rot, 
		CameraAttrBits_LookAt, 
		CameraAttrBits_Num
	};

	using CameraAttrs = std::bitset<CameraAttrBits_Num>;

	class Camera
	{
	public:
		Camera() {}
		~Camera() = default;

		void flush();

		void setPosition(const glm::vec3& position);
		void setPosition(const SphereCoord &position);
		void setPositionDelta(const glm::vec3& delta);
		void setPositionDelta(const SphereCoord& delta);
		void setLookAt(const glm::vec3& lookAt);

		void setFov(float fov) { m_frustum.setFov(fov); }
		void setAspect(float aspect) { m_frustum.setAspect(aspect); }
		void setNearFar(float near, float far) { m_frustum.setNearFar(near, far); }
		void setOrtho(float l, float r, float b, float t) { m_frustum.setOrtho(l, r, b, t); }
		void setViewport(int32_t x, int32_t y, int32_t w, int32_t h) { m_viewport = Viewport{ x, y, w, h }; }

		float near() const { return m_frustum.near(); }
		float far() const { return m_frustum.far(); }
		float fov() const { return m_frustum.fov(); }

		float topEdge() const { return m_frustum.top(); }
		float bottomEdge() const { return m_frustum.bottom(); }
		float leftEdge() const { return m_frustum.left(); }
		float rightEdge() const { return m_frustum.right(); }

		const glm::mat4& viewMat() const { return m_pose.ViewMat; }
		const glm::mat4& prjMat() const { return m_frustum.getPerspectMat(); }
		const glm::mat4& orthoMat() const { return m_frustum.orthoMat(); }
		const Viewport&  viewport() const { return m_viewport; }

		const glm::vec3& right() const { return m_pose.Right; }
		const glm::vec3& up()    const { return m_pose.Up; }
		const glm::vec3& front() const { return m_pose.Front; }

		void attrChange(CameraAttrBits bit) { m_attrBits.set(bit); }		

		Transform& transform() { return m_transform; }
		const Transform& transform() const { return m_transform; }
		const CPosition& position() const { return m_transform.Position; }
	public:
		/*
		* @brief: calculate view matrix by specific input
		*/
		static glm::mat4 GetViewMat(const SphereCoord &position, const glm::vec3 &lookAt = glm::vec3(0.f));
		static glm::mat4 GetViewMat(const glm::vec3 &position, const glm::vec3 &lookAt = glm::vec3(0.f));
		static glm::mat4 GetOrtho(float l, float r, float b, float t);
		static glm::mat4 GetViewportMat(const Viewport &vp);

	private:
		void calcViewMat();
		void calcCameraPose();

	private:
		Frustum m_frustum{};
		Viewport m_viewport{};
		Transform m_transform{};
		CameraPose m_pose{};
		CameraAttrs m_attrBits{};
	};
}

#endif