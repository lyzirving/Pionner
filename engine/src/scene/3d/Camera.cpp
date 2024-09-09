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

	void Camera::update(Ref<RenderContext>& context)
	{
		setUp(context);

		if (anyChange())
		{
			flush();

			m_unimData.ViewMat = viewMat();
			m_unimData.PrjMat = prjMat();
			m_unimData.OrthoMat = orthoMat();
			m_unimData.CameraPosition = position();
			m_unimData.FrustumFar = frustFar();
			m_unimData.PrjType = prjType();
			m_unimData.serialize();

			context->uploadData(m_unimData.Block.getBuffer()->as<void*>(), m_unimData.Block.getByteUsed(), m_unimBuff);
		}
	}

	void Camera::setPosition(const glm::vec3& position)
	{
		if (m_transform.setPosition(position))
		{
			m_attrBits.set(CameraAttrBits_Pos);
		}
	}

	void Camera::setEuler(const glm::vec3& euler)
	{
		if (m_transform.setEuler(euler))
		{
			m_attrBits.set(CameraAttrBits_Rot);
		}
	}

	void Camera::addTranslation(const glm::vec3& delta)
	{
		m_transform.addTranslation(delta);
		m_attrBits.set(CameraAttrBits_Pos);
	}

	void Camera::addEuler(const glm::vec3& delta)
	{
		m_transform.addEuler(delta);
		m_attrBits.set(CameraAttrBits_Rot);
	}

	void Camera::flush()
	{
		calcViewMat();
		m_persFrustum.flush();
		m_orthoFrustum.flush();
	}

	void Camera::setUp(Ref<RenderContext>& context)
	{
		if (!m_unimBuff)
		{
			m_unimData.obtainBlock();
			m_unimBuff = UniformBuffer::Create(context, m_unimData.Block.getByteUsed(), UBBinding_Camera, BufferUsage::Dynamic);
		}
	}

	void Camera::calcViewMat()
	{
		if (m_attrBits.any())
		{			
			m_transform.flush();
			calcCameraPose();
			m_viewMat = glm::lookAt(m_transform.position(), m_transform.position() + m_viewDir * 10.f, m_up);
			m_attrBits.reset();
		}
	}

	void Camera::calcCameraPose()
	{
		m_viewDir = -World::Forward;
		m_viewDir = m_transform.rotMat() * glm::vec4(m_viewDir, 0.f);

		// compute the right and up vector by view direction and world up
		m_right = glm::normalize(glm::cross(m_viewDir, glm::vec3(0.f, 1.f, 0.f)));
		m_up = glm::normalize(glm::cross(m_right, m_viewDir));
		m_front = -m_viewDir;
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
}