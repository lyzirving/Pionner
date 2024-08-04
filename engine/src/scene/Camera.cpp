#include "Camera.h"

#include "gfx/renderer/Renderer.h"
#include "gfx/debug/GDebugger.h"

#include "ui/UiDef.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Camera"

namespace pio
{
	Ref<Camera> Camera::Main{ nullptr };

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

	Camera::Camera(const Camera& rhs) : Asset(rhs),
		m_transform(rhs.m_transform), m_pose(rhs.m_pose),
		m_attrBits(rhs.m_attrBits), m_prjType(rhs.m_prjType),
		m_persFrustum(rhs.m_persFrustum), m_orthoFrustum(rhs.m_orthoFrustum),
		m_target(rhs.m_target)
	{
	}

	Camera& Camera::operator=(const Camera& rhs)
	{
		if (this != &rhs)
		{
			Asset::operator=(rhs);
			m_transform = rhs.m_transform;
			m_pose = rhs.m_pose;
			m_attrBits = rhs.m_attrBits;
			m_prjType = rhs.m_prjType;
			m_persFrustum = rhs.m_persFrustum;
			m_orthoFrustum = rhs.m_orthoFrustum;
			m_target = rhs.m_target;
		}
		return *this;
	}

	void Camera::flush()
	{
		calcViewMat();
		m_persFrustum.flush();
		m_orthoFrustum.flush();
	}

	void Camera::initSkybox(const std::string& name, AssetFmt fmt)
	{
		m_sky = CreateRef<Skybox>(name, fmt);
		auto sk = m_sky;
		Renderer::SubmitTask([sk]() mutable { sk->prepare(); });
	}

	Camera Camera::clone()
	{
		Camera cam{};
		cam.m_name = this->m_name;
		cam.m_parentHandle = this->m_parentHandle;

		cam.m_transform = this->m_transform;
		cam.m_pose = this->m_pose;
		cam.m_attrBits = this->m_attrBits;
		cam.m_prjType = this->m_prjType;
		cam.m_persFrustum = this->m_persFrustum;
		cam.m_orthoFrustum = this->m_orthoFrustum;
		cam.m_target = this->m_target;

		cam.m_sky = this->m_sky;
		return cam;
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

	Ray Camera::screenPointToRay(const glm::vec2& screenPt, const LayoutParams& param, bool bDraw)
	{
		auto vpPoint = Math::ScreenPtToViewportPt(screenPt, param);

		float far = frustFar();
		float near = frustNear();
		float left = frustLeft();
		float right = frustRight();
		float top = frustTop();
		float bottom = frustBottom();
		// camera position in world space
		glm::vec3 camPos = position();
		const Viewport& vp = viewport();
		const glm::mat4& vMat = viewMat();

		// origin of OpenGL's viewport is left-bottom.
		glm::vec2 pt = vpPoint;
		// transform pt from screen to CVV
		pt.x = (pt.x / (float)vp.w()) * 2.f - 1.f;
		pt.y = (pt.y / (float)vp.h()) * 2.f - 1.f;

		// transform pt to project plane
		pt.x = (pt.x + 1.f) * 0.5f * (right - left) + left;
		pt.y = (pt.y + 1.f) * 0.5f * (top - bottom) + bottom;

		// pt in camera space on near plane
		glm::vec3 camPt = glm::vec3(pt.x, pt.y, -near);

		// --------------transform pt in camera space into world space-------------
		// get rotation matrix
		glm::mat3 rotMat = glm::mat3(vMat[0].x, vMat[0].y, vMat[0].z,
			vMat[1].x, vMat[1].y, vMat[1].z,
			vMat[2].x, vMat[2].y, vMat[2].z);
		glm::mat3 invRot = glm::inverse(rotMat);
		camPt = invRot * camPt;
		camPt += camPos;

		if (Renderer::GetConfig().Debugger.Raycast && bDraw)
		{
			// adjust the point in frustum for GDebuger visualization
			glm::vec3 pos = camPos;
			glm::vec3 dstPos = camPt;
			pos = pos - front() * near;
			dstPos = dstPos - front() * near;
			GDebugger::Get()->addLine(Ray(pos, glm::normalize(dstPos - pos)));
		}

		return Ray(camPos, glm::normalize(camPt - camPos), camPt);
	}

	glm::vec3 Camera::screenPointToNearPlane(const glm::vec2& screenPt, const LayoutParams& param)
	{
		auto vpPoint = Math::ScreenPtToViewportPt(screenPt, param);

		float far = frustFar();
		float near = frustNear();
		float left = frustLeft();
		float right = frustRight();
		float top = frustTop();
		float bottom = frustBottom();
		// camera position in world space
		glm::vec3 camPos = position();
		const Viewport& vp = viewport();
		const glm::mat4& vMat = viewMat();

		// origin of OpenGL's viewport is left-bottom.
		glm::vec2 pt = vpPoint;
		// transform pt from screen to CVV
		pt.x = (pt.x / (float)vp.w()) * 2.f - 1.f;
		pt.y = (pt.y / (float)vp.h()) * 2.f - 1.f;

		// transform pt to project plane
		pt.x = (pt.x + 1.f) * 0.5f * (right - left) + left;
		pt.y = (pt.y + 1.f) * 0.5f * (top - bottom) + bottom;

		// pt in camera space on near plane
		glm::vec3 camPt = glm::vec3(pt.x, pt.y, -near);

		// --------------transform pt in camera space into world space-------------
		// get rotation matrix
		glm::mat3 rotMat = glm::mat3(vMat[0].x, vMat[0].y, vMat[0].z,
			vMat[1].x, vMat[1].y, vMat[1].z,
			vMat[2].x, vMat[2].y, vMat[2].z);
		glm::mat3 invRot = glm::inverse(rotMat);
		camPt = invRot * camPt;
		camPt += camPos;

		return camPt;
	}

	template<>
	bool Asset::is<Camera>() const { return getAssetType() == AssetType::Camera; }
}