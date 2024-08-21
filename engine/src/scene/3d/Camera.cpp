#include "Camera.h"

#include "gfx/renderer/RenderContext.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Camera"

namespace pio
{
	void Camera::culling(PendingData& pendingData)
	{
		//TODO
	}

	void Camera::setUp(Ref<RenderContext>& context)
	{
		flush();

		m_data.setUp(context);

		m_data.m_uniformData.ViewMat = viewMat();
		m_data.m_uniformData.PrjMat = prjMat();
		m_data.m_uniformData.OrthoMat = orthoMat();
		m_data.m_uniformData.CameraPosition = position();
		m_data.m_uniformData.FrustumFar = frustFar();
		m_data.m_uniformData.PrjType = prjType();
		m_data.m_uniformData.serialize();

		context->uploadData(m_data.m_uniformData.Block.getBuffer()->as<void*>(), m_data.m_uniformData.Block.getByteUsed(), m_data.m_uniformBuffer);
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
	bool Asset::is<Camera>() const { return type() == AssetType::Camera; }
}