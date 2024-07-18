#include "Camera.h"

#include "core/math/MathLib.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Camera"

namespace pio
{
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
	}

	UniformBlock CameraUD::CreateBlock()
	{
		UniformBlock block;
		block.pushBack("ViewMat", UniformBlock::CreateData(UniformDataType::Mat4, "ViewMat"));
		block.pushBack("PrjMat", UniformBlock::CreateData(UniformDataType::Mat4, "PrjMat"));
		block.pushBack("OrthoMat", UniformBlock::CreateData(UniformDataType::Mat4, "OrthoMat"));
		block.pushBack("CameraPosition", UniformBlock::CreateData(UniformDataType::Vec3, "CameraPosition"));
		block.pushBack("FrustumFar", UniformBlock::CreateData(UniformDataType::Float, "FrustumFar"));
		block.calculate();
		//LOGD("block CameraUD byte used[%u]", block.getByteUsed());
		return block;
	}

	void Camera::flush()
	{
		calcViewMat();
		m_frustum.calcPrj();
		m_frustum.calcOrtho();
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
		if(m_transform.Position != position)
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
			glm::quat quat = glm::toQuat(glm::mat3(m_pose.Right, m_pose.Up, m_pose.Front));
			m_transform.Euler = glm::degrees(glm::eulerAngles(quat));
		}
	}

	glm::mat4 Camera::GetViewMat(const SphereCoord &position, const glm::vec3 &lookAt)
	{
		glm::vec3 pos = position.to();
		glm::vec3 viewDir = glm::normalize(lookAt - pos);
		// compute the right and up vector by view direction and world up
		glm::vec3 right = glm::normalize(glm::cross(viewDir, glm::vec3(0.f, 1.f, 0.f)));
		glm::vec3 up = glm::normalize(glm::cross(right, viewDir));
		return glm::lookAt(pos, lookAt, up);
	}

	glm::mat4 Camera::GetViewMat(const glm::vec3 &position, const glm::vec3 &lookAt)
	{
		glm::vec3 viewDir = glm::normalize(lookAt - position);
		// compute the right and up vector by view direction and world up
		glm::vec3 right = glm::normalize(glm::cross(viewDir, glm::vec3(0.f, 1.f, 0.f)));
		glm::vec3 up = glm::normalize(glm::cross(right, viewDir));
		return glm::lookAt(position, lookAt, up);
	}

	glm::mat4 Camera::GetOrtho(float l, float r, float b, float t)
	{
		return glm::ortho(l, r, b, t);
	}

	glm::mat4 Camera::GetViewportMat(const Viewport &vp)
	{
		glm::vec4 col0 = glm::vec4(float(vp.Width) / 2.f, 0.f, 0.f, 0.f);
		glm::vec4 col1 = glm::vec4(0.f, float(vp.Height) / 2.f, 0.f, 0.f);
		glm::vec4 col2 = glm::vec4(0.f, 0.f, 0.5f, 0.f);
		glm::vec4 col3 = glm::vec4(float(vp.X) + float(vp.Width) / 2.f,
								   float(vp.Y) + float(vp.Height) / 2.f,
								   0.5f, 1.f);
		return glm::mat4(col0, col1, col2, col3);
	}
}