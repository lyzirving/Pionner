#ifndef __PIONNER_SCENE_3D_CAMERA_H__
#define __PIONNER_SCENE_3D_CAMERA_H__

#include "Frustum.h"

#include "asset/Asset.h"
#include "gfx/resource/CameraData.h"

namespace pio
{	
	class CameraNode;
	class RenderContext;	
	struct RenderingNodes;
	struct CameraComponent;

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
		CameraAttrBits_PrjType,
		CameraAttrBits_Num
	};

	using CameraAttrs = std::bitset<CameraAttrBits_Num>;

	class Camera : public Asset
	{
		PIO_DEFINE_ASSET_TYPE(AssetType::Camera)
	public:
		Camera() : Asset() {}
		~Camera() = default;

		void culling(RenderingNodes& renderingNodes);
		void flush();

		void setPosition(const glm::vec3& position);
		void setEuler(const glm::vec3& euler);

		void addTranslation(const glm::vec3& delta);
		void addEuler(const glm::vec3& delta);

		void setPrjType(ProjectionType type);
		void setAspect(float aspect) { m_persFrustum.setAspect(aspect); m_orthoFrustum.setAspect(aspect); }
		void setNear(float near) { m_persFrustum.setNear(near); m_orthoFrustum.setNear(near); }
		void setFar(float far) { m_persFrustum.setFar(far); m_orthoFrustum.setFar(far); }

		void setFov(float fov) { m_persFrustum.setFov(fov); }
		void setSize(float size) { m_orthoFrustum.setSize(size); }

		bool anyChange() const { return m_attrBits.any() || m_persFrustum.bDirty() || m_orthoFrustum.bDirty(); }
		void attrChange(CameraAttrBits bit) { m_attrBits.set(bit); }
		void setClearFlag(CameraClearFlags f) { m_clearFlag = f; }

		ProjectionType prjType() const { return m_prjType; }
		float fov() const { return m_persFrustum.fov(); }
		float size() const { return m_orthoFrustum.size(); }

		const glm::vec3& right() const { return m_right; }
		const glm::vec3& up()    const { return m_up; }
		const glm::vec3& front() const { return m_front; }
		const glm::vec3& viewDir() const { return m_viewDir; }

		float frustNear()   const { return m_prjType == ProjectionType_Perspective ? m_persFrustum.near() : m_orthoFrustum.near(); }
		float frustFar()    const { return m_prjType == ProjectionType_Perspective ? m_persFrustum.far() : m_orthoFrustum.far(); }
		float frustTop()    const { return m_prjType == ProjectionType_Perspective ? m_persFrustum.top() : m_orthoFrustum.top(); }
		float frustBottom() const { return m_prjType == ProjectionType_Perspective ? m_persFrustum.bottom() : m_orthoFrustum.bottom(); }
		float frustLeft()   const { return m_prjType == ProjectionType_Perspective ? m_persFrustum.left() : m_orthoFrustum.left(); }
		float frustRight()  const { return m_prjType == ProjectionType_Perspective ? m_persFrustum.right() : m_orthoFrustum.right(); }

		const glm::mat4& viewMat()  const { return m_viewMat; }
		const glm::mat4& prjMat()   const { return m_persFrustum.mat(); }
		const glm::mat4& orthoMat() const { return m_orthoFrustum.mat(); }

		const glm::vec3& position() const { return m_transform.position(); }
		CameraClearFlags clearFlag() const { return m_clearFlag; }

	private:
		void calcViewMat();
		void calcCameraPose();

	public:
		static glm::mat4 ViewportMat(const Viewport& vp);
		static glm::vec3 GetViewDir(const Ref<CameraNode> &node);
		static glm::vec3 GetMotionDir(const Ref<CameraNode>& node, uint16_t key);

	private:	
		Transform3D m_transform{};
		// camera's +x/+y/+z axis
		glm::vec3 m_right{ 1.f, 0.f, 0.f }, m_up{ 0.f, 1.f, 0.f }, m_front{ 0.f, 0.f, 1.f };
		glm::vec3 m_viewDir{ -World::Forward };
		glm::mat4 m_viewMat{ 1.f };
		CameraAttrs m_attrBits{};

		ProjectionType m_prjType{ ProjectionType_Perspective };
		PerspectiveFrustum m_persFrustum;
		OrthographicFrustum m_orthoFrustum;

		CameraClearFlags m_clearFlag{ CameraClearFlag_Skybox };
	};

	template<>
	bool Asset::is<Camera>() const;
}

#endif