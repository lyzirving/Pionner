#include "Camera.h"

#include "asset/AssetMgr.h"
#include "event/KeyCodes.h"

#include "scene/Components.h"
#include "scene/node/CameraNode.h"

#include "gfx/renderer/RenderContext.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Camera"

namespace pio
{
	void Camera::culling(RenderingNodes& renderingNodes)
	{
		//TODO
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

	void Camera::setLookAt(const glm::vec3& lookAt)
	{
		if(m_lookAt != lookAt)
		{
			m_lookAt = lookAt;
			m_attrBits.set(CameraAttrBits_LookAt);
		}
	}

	void Camera::setMotionMode(CameraMotionMode mode)
	{
		if(m_motionMode != mode)
		{
			m_motionMode = mode;
			m_attrBits.set(CameraAttrBits_MotionMode);
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

	void Camera::setPrjType(ProjectionType type)
	{
		if (m_prjType != type)
		{
			m_prjType = type;
			m_attrBits.set(CameraAttrBits_PrjType);
		};
	}

	void Camera::flush()
	{
		calcViewMat();
		m_persFrustum.flush();
		m_orthoFrustum.flush();
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
		if(m_motionMode == CameraMotionMode_Free)
		{
			m_viewDir = -World::Forward;
			m_viewDir = m_transform.rotMat() * glm::vec4(m_viewDir, 0.f);			
		}
		else if(m_motionMode == CameraMotionMode_FixTarget)
		{
			m_viewDir = glm::normalize(m_lookAt - m_transform.position());
		}
		else
		{
			LOGE("err! invalid camera motion mode[%u]", m_motionMode);
		}

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

	glm::vec3 Camera::GetViewDir(const Ref<CameraNode>& node)
	{
		return node->camera()->viewDir();
	}

	glm::vec3 Camera::GetMotionDir(const Ref<CameraNode>& node, uint16_t key)
	{
		glm::vec3 dir(0.f);
		switch (key)
		{
			case Key::A:
			{
				dir = -node->camera()->right();
				break;
			}
			case Key::D:
			{
				dir = node->camera()->right();
				break;
			}
			case Key::W:
			{
				dir = node->camera()->up();
				break;
			}
			case Key::S:
			{
				dir = -node->camera()->up();
				break;
			}
			default:
				break;
		}
		return dir;
	}

	glm::vec3 Camera::BillBoardRotate(const Ref<CameraNode>& node)
	{
		auto rotMat = glm::mat3(glm::inverse(node->camera()->viewMat()));
		return glm::degrees(glm::eulerAngles(glm::quat_cast(rotMat)));
	}

	template<>
	bool Asset::is<Camera>() const { return assetType() == AssetType::Camera; }
}