#include "Camera.h"

#include "core/math/MathLib.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Camera"

#define CHANGE_RATIO_CAM_POS (0.75f)

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

	Camera::CameraImpl::CameraImpl()
	{
	}

	Camera::CameraImpl::CameraImpl(const CameraImpl &rhs) : m_camPos(rhs.m_camPos), m_camPosSpherical(rhs.m_camPosSpherical)
		, m_lookAt(rhs.m_lookAt), m_camRight(rhs.m_camRight), m_camUp(rhs.m_camUp)
		, m_viewMat(rhs.m_viewMat)
	{
	}

	Camera::CameraImpl &Camera::CameraImpl::operator=(const CameraImpl &rhs)
	{
		if (this != &rhs)
		{
			m_camPos = rhs.m_camPos;
			m_camPosSpherical = rhs.m_camPosSpherical;
			m_lookAt = rhs.m_lookAt;
			m_camRight = rhs.m_camRight;
			m_camUp = rhs.m_camUp;
			m_viewMat = rhs.m_viewMat;
		}
		return *this;
	}

	Camera::CameraImpl::~CameraImpl() = default;

	Camera::CameraImpl::CamState Camera::CameraImpl::acquireState()
	{
		CamState state{};
		state.m_position = m_camPos;
		state.m_lookAt = m_lookAt;
		return state;
	}

	void Camera::CameraImpl::applyState(const Camera::CameraImpl::CamState &state)
	{
		setPosition(state.m_position);
		setLookAt(state.m_lookAt);
	}

	void Camera::CameraImpl::addPosDiff(float thetaDiff, float phiDiff, float rDiff)
	{
		float theta = m_camPosSpherical.getTheta();
		float phi = m_camPosSpherical.getPhi();
		float r = m_camPosSpherical.getRadius();

		theta += thetaDiff;
		phi += phiDiff;
		r += rDiff;

		m_camPosSpherical.set(theta, phi, r);
		m_camPosSpherical.checkRange();

		m_camPos = m_camPosSpherical.toCartesian();
	}

	void Camera::CameraImpl::addRadiusDiff(float delta)
	{
		float theta = m_camPosSpherical.getTheta();
		float phi = m_camPosSpherical.getPhi();
		float r = m_camPosSpherical.getRadius();

		r += delta;

		m_camPosSpherical.set(theta, phi, r);
		m_camPosSpherical.checkRange();

		m_camPos = m_camPosSpherical.toCartesian();
	}

	void Camera::CameraImpl::setPosition(float theta, float phi, float r)
	{
		m_camPosSpherical.set(theta, phi, r);
		m_camPosSpherical.checkRange();

		m_camPos = m_camPosSpherical.toCartesian();
	}

	void Camera::CameraImpl::setPosition(const SphereCoord &position)
	{
		m_camPosSpherical = position;
		m_camPosSpherical.checkRange();

		m_camPos = m_camPosSpherical.toCartesian();
	}

	void Camera::CameraImpl::setPosition(const glm::vec3 &pos)
	{
		m_camPos = pos;
		m_camPosSpherical.applyCartesian(pos);
	}

	void Camera::CameraImpl::setLookAt(const glm::vec3 &lookAt)
	{
		m_lookAt = lookAt;
	}

	void Camera::CameraImpl::calcViewMat()
	{
		glm::vec3 viewDir = glm::normalize(m_lookAt - m_camPos);
		// compute the right and up vector by view direction and world up
		m_camRight = glm::normalize(glm::cross(viewDir, glm::vec3(0.f, 1.f, 0.f)));
		m_camUp = glm::normalize(glm::cross(m_camRight, viewDir));

		m_viewMat = glm::lookAt(m_camPos, m_lookAt, m_camUp);
	}

	glm::mat4 Camera::CameraImpl::CalcViewMat(const SphereCoord &position, const glm::vec3 &lookAt)
	{
		glm::vec3 pos = position.toCartesian();
		glm::vec3 viewDir = glm::normalize(lookAt - pos);
		// compute the right and up vector by view direction and world up
		glm::vec3 right = glm::normalize(glm::cross(viewDir, glm::vec3(0.f, 1.f, 0.f)));
		glm::vec3 up = glm::normalize(glm::cross(right, viewDir));
		return glm::lookAt(pos, lookAt, up);
	}

	glm::mat4 Camera::CameraImpl::CalcViewMat(const glm::vec3 &position, const glm::vec3 &lookAt)
	{
		glm::vec3 viewDir = glm::normalize(lookAt - position);
		// compute the right and up vector by view direction and world up
		glm::vec3 right = glm::normalize(glm::cross(viewDir, glm::vec3(0.f, 1.f, 0.f)));
		glm::vec3 up = glm::normalize(glm::cross(right, viewDir));
		return glm::lookAt(position, lookAt, up);
	}

	Camera::Camera() : m_id(CameraId::MAIN)
	{
	}

	Camera::Camera(CameraId id) : m_id(id)
	{
	}

	Camera::Camera(const Camera &rhs) : m_visible(rhs.m_visible), m_id(rhs.m_id), m_camImpl(rhs.m_camImpl),
		m_frustum(rhs.m_frustum), m_viewport(rhs.m_viewport)
	{
	}

	Camera &Camera::operator=(const Camera &rhs)
	{
		if (this != &rhs)
		{
			m_visible = rhs.m_visible;
			m_id = rhs.m_id;
			m_camImpl = rhs.m_camImpl;
			m_frustum = rhs.m_frustum;
			m_viewport = rhs.m_viewport;
		}
		return *this;
	}

	Camera::~Camera() = default;

	void Camera::addPosDiff(float deltaX, float deltaY)
	{
		m_camImpl.addPosDiff(-deltaY * CHANGE_RATIO_CAM_POS, -deltaX * CHANGE_RATIO_CAM_POS, 0.f);
	}

	void Camera::addRadiusDiff(float delta)
	{
		m_camImpl.addRadiusDiff(delta);
	}

	void Camera::flush()
	{
		m_camImpl.calcViewMat();
		m_frustum.calcPrj();
		m_frustum.calcOrtho();
	}

	glm::mat4 Camera::GetViewMat(const SphereCoord &position, const glm::vec3 &lookAt)
	{
		return CameraImpl::CalcViewMat(position, lookAt);
	}

	glm::mat4 Camera::GetViewMat(const glm::vec3 &position, const glm::vec3 &lookAt)
	{
		return CameraImpl::CalcViewMat(position, lookAt);
	}

	glm::mat4 Camera::GetOrtho(float l, float r, float b, float t)
	{
		return glm::ortho(l, r, b, t);
	}
}