#ifndef __PIONNER_CORE_MATH_CAMERA_H__
#define __PIONNER_CORE_MATH_CAMERA_H__

#include "PerspectiveFrustum.h"
#include "OrthographicFrustum.h"

#include "asset/Asset.h"
#include "gfx/renderer/RenderState.h"
#include "gfx/rhi/Uniform.h"
#include "core/math/Transform.h"

namespace pio
{
	struct CameraUD
	{
		PIO_UNIFORM_STRUCT_DECLARE(CameraUD)

		glm::mat4 ViewMat{ 1.f };
		glm::mat4 PrjMat{ 1.f };
		glm::mat4 OrthoMat{ 1.f };
		glm::vec3 CameraPosition{ 0.f };
		float FrustumFar{ 0.f };
		ProjectionType PrjType{ ProjectionType_Perspective };
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

	class Camera : public Asset
	{
		OVERRIDE_ASSET_TYPE(AssetType::Camera)
	public:
		Camera() {}
		~Camera() = default;

		void flush();

		void setPosition(const glm::vec3& position);
		void setPosition(const SphereCoord& position);
		void setPositionDelta(const glm::vec3& delta);
		void setPositionDelta(const SphereCoord& delta);
		void setLookAt(const glm::vec3& lookAt);

		void setPrjType(ProjectionType type) { m_prjType = type; }
		void setAspect(float aspect) { m_persFrustum.setAspect(aspect); m_orthoFrustum.setAspect(aspect); }
		void setNear(float near) { m_persFrustum.setNear(near); m_orthoFrustum.setNear(near); }
		void setFar(float far) { m_persFrustum.setFar(far); m_orthoFrustum.setFar(far); }

		void setFov(float fov) { m_persFrustum.setFov(fov); }
		void setSize(float size) { m_orthoFrustum.setSize(size); }

		void setViewport(int32_t x, int32_t y, int32_t w, int32_t h) { m_viewport = Viewport{ x, y, w, h }; }

		ProjectionType prjType() const { return m_prjType; }
		float fov() const { return m_persFrustum.fov(); }
		float size() const { return m_orthoFrustum.size(); }

		float frustNear()   const { return m_prjType == ProjectionType_Perspective ? m_persFrustum.near() : m_orthoFrustum.near(); }
		float frustFar()    const { return m_prjType == ProjectionType_Perspective ? m_persFrustum.far() : m_orthoFrustum.far(); }
		float frustTop()    const { return m_prjType == ProjectionType_Perspective ? m_persFrustum.top() : m_orthoFrustum.top(); }
		float frustBottom() const { return m_prjType == ProjectionType_Perspective ? m_persFrustum.bottom() : m_orthoFrustum.bottom(); }
		float frustLeft()   const { return m_prjType == ProjectionType_Perspective ? m_persFrustum.left() : m_orthoFrustum.left(); }
		float frustRight()  const { return m_prjType == ProjectionType_Perspective ? m_persFrustum.right() : m_orthoFrustum.right(); }

		const glm::mat4& viewMat()  const { return m_pose.ViewMat; }
		const glm::mat4& prjMat()   const { return m_persFrustum.mat(); }
		const glm::mat4& orthoMat() const { return m_orthoFrustum.mat(); }
		const Viewport& viewport() const { return m_viewport; }

		const glm::vec3& right() const { return m_pose.Right; }
		const glm::vec3& up()    const { return m_pose.Up; }
		const glm::vec3& front() const { return m_pose.Front; }

		void attrChange(CameraAttrBits bit) { m_attrBits.set(bit); }

		Transform& transform() { return m_transform; }
		CPosition& position() { return m_transform.Position; }

		const Transform& transform() const { return m_transform; }
		const CPosition& position()  const { return m_transform.Position; }
	public:
		/*
		* @brief: calculate view matrix by specific input
		*/
		static glm::mat4 GetViewMat(const SphereCoord& position, const glm::vec3& lookAt = glm::vec3(0.f));
		static glm::mat4 GetViewMat(const glm::vec3& position, const glm::vec3& lookAt = glm::vec3(0.f));
		static glm::mat4 GetOrtho(float l, float r, float b, float t);
		static glm::mat4 GetViewportMat(const Viewport& vp);

	private:
		void calcViewMat();
		void calcCameraPose();

	private:
		Viewport    m_viewport{};
		Transform   m_transform{};
		CameraPose  m_pose{};
		CameraAttrs m_attrBits{};

		ProjectionType m_prjType{ ProjectionType_Perspective };
		PerspectiveFrustum m_persFrustum;
		OrthographicFrustum m_orthoFrustum;
	};

	template<>
	bool Asset::is<Camera>() const;
}

#endif