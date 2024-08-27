#ifndef __PIONNER_SCENE_3D_CAMERA_H__
#define __PIONNER_SCENE_3D_CAMERA_H__

#include "Frustum.h"

#include "asset/Asset.h"
#include "base/Transform.h"
#include "gfx/rhi/UniformBuffer.h"

namespace pio
{
	struct RenderingEntities;
	struct CameraComponent;
	class RenderContext;
	class Entity;

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

	enum CameraClearFlags : uint8_t
	{
		CameraClearFlag_Skybox = 0,
		CameraClearFlag_Color,
		//TODO
		CameraClearFlag_DepthOnly,
		CameraClearFlag_NoClear,
		CameraClearFlag_Num
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
		Camera() : Asset() {}
		~Camera() = default;

		void culling(RenderingEntities& renderingEntities);
		void setUp(Ref<RenderContext>& context);
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

		bool anyChange() const { return m_attrBits.any() || m_persFrustum.bDirty() || m_orthoFrustum.bDirty(); }
		void attrChange(CameraAttrBits bit) { m_attrBits.set(bit); }
		void setClearFlag(CameraClearFlags f) { m_clearFlag = f; }
		void setDepth(int32_t depth) { m_depth = depth; }

		int32_t depth() const { return m_depth; }
		ProjectionType prjType() const { return m_prjType; }
		float fov() const { return m_persFrustum.fov(); }
		float size() const { return m_orthoFrustum.size(); }

		const glm::vec3& right() const { return m_pose.Right; }
		const glm::vec3& up()    const { return m_pose.Up; }
		const glm::vec3& front() const { return m_pose.Front; }

		float frustNear()   const { return m_prjType == ProjectionType_Perspective ? m_persFrustum.near() : m_orthoFrustum.near(); }
		float frustFar()    const { return m_prjType == ProjectionType_Perspective ? m_persFrustum.far() : m_orthoFrustum.far(); }
		float frustTop()    const { return m_prjType == ProjectionType_Perspective ? m_persFrustum.top() : m_orthoFrustum.top(); }
		float frustBottom() const { return m_prjType == ProjectionType_Perspective ? m_persFrustum.bottom() : m_orthoFrustum.bottom(); }
		float frustLeft()   const { return m_prjType == ProjectionType_Perspective ? m_persFrustum.left() : m_orthoFrustum.left(); }
		float frustRight()  const { return m_prjType == ProjectionType_Perspective ? m_persFrustum.right() : m_orthoFrustum.right(); }

		const glm::mat4& viewMat()  const { return m_pose.ViewMat; }
		const glm::mat4& prjMat()   const { return m_persFrustum.mat(); }
		const glm::mat4& orthoMat() const { return m_orthoFrustum.mat(); }

		Transform& transform() { return m_transform; }
		Position3d& position() { return m_transform.Position; }

		const Transform& transform() const { return m_transform; }
		const Position3d& position()  const { return m_transform.Position; }
		CameraClearFlags clearFlag() const { return m_clearFlag; }

		Ref<UniformBuffer>& unimBuffer() { return m_data.UnimBuff; }

	private:
		void calcViewMat();
		void calcCameraPose();

	public:
		static glm::mat4 ViewportMat(const Viewport& vp);

	private:	
		Transform m_transform{};
		CameraPose m_pose{};
		CameraAttrs m_attrBits{};

		ProjectionType m_prjType{ ProjectionType_Perspective };
		PerspectiveFrustum m_persFrustum;
		OrthographicFrustum m_orthoFrustum;

		CameraClearFlags m_clearFlag{ CameraClearFlag_Skybox };

		int32_t m_depth{ 0 };
		
	private:
		struct Data
		{
			CameraUD UnimData{};
			Ref<UniformBuffer> UnimBuff;

			Data() { UnimData.obtainBlock(); }
		};

		Data m_data{};
	};

	template<>
	bool Asset::is<Camera>() const;
}

#endif