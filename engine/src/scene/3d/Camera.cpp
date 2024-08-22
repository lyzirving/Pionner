#include "Camera.h"

#include "asset/AssetMgr.h"

#include "scene/Components.h"
#include "scene/Entity.h"

#include "gfx/renderer/RenderContext.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Camera"

namespace pio
{
	UniformBlock CameraUD::CreateBlock()
	{
		UniformBlock block;
		block.pushBack("ViewMat", UniformBlock::CreateData(UniformDataType::Mat4, "ViewMat"));
		block.pushBack("PrjMat", UniformBlock::CreateData(UniformDataType::Mat4, "PrjMat"));
		block.pushBack("OrthoMat", UniformBlock::CreateData(UniformDataType::Mat4, "OrthoMat"));
		block.pushBack("CameraPosition", UniformBlock::CreateData(UniformDataType::Vec3, "CameraPosition"));
		block.pushBack("FrustumFar", UniformBlock::CreateData(UniformDataType::Float, "FrustumFar"));
		block.pushBack("PrjType", UniformBlock::CreateData(UniformDataType::Int, "PrjType"));
		block.calculate();
		//LOGD("block CameraUD byte used[%u]", block.getByteUsed());
		return block;
	}

	void CameraUD::serialize()
	{
		if (!Block.m_buffer || Block.m_buffer->getCapacity() == 0)
		{
			LOGE("UD buffer is invalid");
			return;
		}

		auto viewMatUD = Block.m_blockItems.get("ViewMat");
		if (viewMatUD)
		{
			Block.m_buffer->writeAt(glm::value_ptr(ViewMat), sizeof(glm::mat4), viewMatUD->getAlignOffset());
		}

		auto prjMatUD = Block.m_blockItems.get("PrjMat");
		if (prjMatUD)
		{
			Block.m_buffer->writeAt(glm::value_ptr(PrjMat), sizeof(glm::mat4), prjMatUD->getAlignOffset());
		}

		auto orthoMatUD = Block.m_blockItems.get("OrthoMat");
		if (orthoMatUD)
		{
			Block.m_buffer->writeAt(glm::value_ptr(OrthoMat), sizeof(glm::mat4), orthoMatUD->getAlignOffset());
		}

		auto cameraPositionUD = Block.m_blockItems.get("CameraPosition");
		if (cameraPositionUD)
		{
			Block.m_buffer->writeAt(&CameraPosition, sizeof(glm::vec3), cameraPositionUD->getAlignOffset());
		}

		auto frustumFarUD = Block.m_blockItems.get("FrustumFar");
		if (frustumFarUD)
		{
			Block.m_buffer->writeAt(&FrustumFar, sizeof(float), frustumFarUD->getAlignOffset());
		}

		auto prjTypeUD = Block.m_blockItems.get("PrjType");
		if (prjTypeUD)
		{
			int32_t val = PrjType;
			Block.m_buffer->writeAt(&val, sizeof(int32_t), prjTypeUD->getAlignOffset());
		}
	}

	void Camera::culling(RenderingEntities& renderingEntities)
	{
		//TODO
	}

	void Camera::setUp(Ref<RenderContext>& context)
	{
		flush();

		if (!m_data.UnimBuff)
		{
			m_data.UnimBuff = UniformBuffer::Create(context, m_data.UnimData.Block.getByteUsed(), UBBinding_Camera, BufferUsage::Dynamic);
		}

		m_data.UnimData.ViewMat = viewMat();
		m_data.UnimData.PrjMat = prjMat();
		m_data.UnimData.OrthoMat = orthoMat();
		m_data.UnimData.CameraPosition = position();
		m_data.UnimData.FrustumFar = frustFar();
		m_data.UnimData.PrjType = prjType();
		m_data.UnimData.serialize();

		context->uploadData(m_data.UnimData.Block.getBuffer()->as<void*>(), m_data.UnimData.Block.getByteUsed(), m_data.UnimBuff);
	}

	void Camera::flush()
	{
		calcViewMat();
		m_persFrustum.flush();
		m_orthoFrustum.flush();
	}

	void Camera::setPosition(const glm::vec3& position)
	{
		if (m_transform.Position != position)
		{
			m_transform.Position = position;
			m_attrBits.set(CameraAttrBits_Pos);
		}
	}

	void Camera::setPosition(const SphereCoord& position)
	{
		if (m_transform.Position != position)
		{
			m_transform.Position = position;
			m_attrBits.set(CameraAttrBits_Pos);
		}
	}

	void Camera::setPositionDelta(const glm::vec3& delta)
	{
		m_transform.Position += delta;
		m_attrBits.set(CameraAttrBits_Pos);
	}

	void Camera::setPositionDelta(const SphereCoord& delta)
	{
		m_transform.Position += delta;
		m_attrBits.set(CameraAttrBits_Pos);
	}

	void Camera::setLookAt(const glm::vec3& lookAt)
	{
		if (m_pose.LookAt != lookAt)
		{
			m_pose.LookAt = lookAt;
			m_attrBits.set(CameraAttrBits_LookAt);
		}
	}

	void Camera::calcViewMat()
	{
		if (m_attrBits.any())
		{
			calcCameraPose();
			m_pose.ViewMat = glm::lookAt(m_transform.Position.ccs(), m_pose.LookAt, m_pose.Up);
			m_attrBits.reset();
		}
	}

	void Camera::calcCameraPose()
	{
		if (m_attrBits.test(CameraAttrBits_Rot))
		{
			float len = glm::length(m_transform.Position.ccs() - m_pose.LookAt);
			if (Math::IsZero(len)) { len += 0.01f; }
			glm::mat4 rotMat = m_transform.Euler.mat();

			m_pose.Right = rotMat * glm::vec4(1.f, 0.f, 0.f, 0.f);
			m_pose.Up = rotMat * glm::vec4(0.f, 1.f, 0.f, 0.f);
			m_pose.Front = rotMat * glm::vec4(0.f, 0.f, 1.f, 0.f);

			m_pose.LookAt = m_transform.Position - m_pose.Front * len;
		}
		else
		{
			glm::vec3 viewDir = glm::normalize(m_pose.LookAt - m_transform.Position.ccs());
			// compute the right and up vector by view direction and world up
			m_pose.Right = glm::normalize(glm::cross(viewDir, glm::vec3(0.f, 1.f, 0.f)));
			m_pose.Up = glm::normalize(glm::cross(m_pose.Right, viewDir));
			m_pose.Front = -viewDir;

			// camera pose axis -> rotation matrix -> quaternion -> euler angle
			glm::mat3 rotMat = glm::mat3(m_pose.Right, m_pose.Up, m_pose.Front);
			glm::quat quat = glm::toQuat(rotMat);
			m_transform.Euler = glm::degrees(glm::eulerAngles(quat));
		}
	}

	glm::mat4 Camera::ViewportMat(const Viewport& vp)
	{
		glm::vec4 col0 = glm::vec4(float(vp.w()) / 2.f, 0.f, 0.f, 0.f);
		glm::vec4 col1 = glm::vec4(0.f, float(vp.h()) / 2.f, 0.f, 0.f);
		glm::vec4 col2 = glm::vec4(0.f, 0.f, 0.5f, 0.f);
		glm::vec4 col3 = glm::vec4(float(vp.x()) + float(vp.w()) / 2.f,
								   float(vp.y()) + float(vp.h()) / 2.f,
								   0.5f, 1.f);
		return glm::mat4(col0, col1, col2, col3);
	}

	template<>
	bool Asset::is<Camera>() const { return assetType() == AssetType::Camera; }

	namespace CameraUtils
	{
		std::vector<Ref<Camera>> FetchCameras(const std::list<Ref<Entity>>& entities)
		{
			std::vector<Ref<Camera>> cameras;
			for (auto& ent : entities)
			{				
				if (ent->has<CameraComponent>())
				{
					auto* comp = ent->getComponent<CameraComponent>();
					auto cam = AssetMgr::GetRuntimeAsset<Camera>(comp->Handle);
					CameraUtils::Update(comp, cam);
					cameras.push_back(cam);
				}				
			}
			return cameras;
		}

		void Update(CameraComponent* comp, Ref<Camera>& camera)
		{
		}
	}

}