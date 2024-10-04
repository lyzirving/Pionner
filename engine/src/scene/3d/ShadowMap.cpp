#include "ShadowMap.h"

#include "Camera.h"
#include "Frustum.h"
#include "ShadowMapFrameBuffer.h"

#include "gfx/renderer/RenderContext.h"
#include "gfx/resource/LightData.h"

#include "scene/Components.h"
#include "scene/node/LightNode.h"
#include "scene/node/CameraNode.h"

#include <limits>

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "ShadowMap"

namespace pio
{
	ShadowMap::ShadowMap(Ref<RenderContext>& context) : LightTechBase()
	{
		m_lightCam = CreateRef<Camera>();
		m_frameBuff = CreateRef<ShadowMapFrameBuffer>(context);

		m_UData = CreateRef<DirectionalLightShadowDataUD>();
		m_UBuffer = UniformBuffer::Create(context, m_UData->Block.getByteUsed(), UBBinding_DirectionalLightShadow, BufferUsage::Dynamic);
	}

	void ShadowMap::update(Ref<RenderContext>& context, Ref<CameraNode>& camNode, Ref<DirectionalLightNode>& lightNode)
	{
		const auto& lightDir = lightNode->direction();
		glm::vec3 lightPos{ 0.f };
		m_lightCam->setPosition(lightPos);
		m_lightCam->setLookAt(lightPos + lightDir * 10.f);
		m_lightCam->setMotionMode(CameraMotionMode_FixTarget);
		m_lightCam->flush();
		auto lightMat = m_lightCam->viewMat();

		auto& cam = camNode->camera();
		glm::mat4 viewMatInv = cam->viewMat();
		viewMatInv = glm::inverse(viewMatInv);

		float aspect = cam->aspect();// width / height
		float fov = cam->fov();
		float tanHalfFov = std::tan(glm::radians(fov * 0.5f));

		float absNear = cam->frustNear();
		float absFar = cam->frustFar();

		glm::vec3 corners[8];
		float yn = absNear * tanHalfFov;
		float xn = aspect * yn;

		corners[0] = lightMat * viewMatInv * glm::vec4(-xn, -yn, -absNear, 1.f);//near left-bottom 
		corners[1] = lightMat * viewMatInv * glm::vec4(-xn, yn, -absNear, 1.f); //near left-top
		corners[2] = lightMat * viewMatInv * glm::vec4(xn, -yn, -absNear, 1.f); //near right-bottom
		corners[3] = lightMat * viewMatInv * glm::vec4(xn, yn, -absNear, 1.f);  //near right-top

		float yf = absFar * tanHalfFov;
		float xf = aspect * yf;

		corners[4] = lightMat * viewMatInv * glm::vec4(-xf, -yf, -absFar, 1.f);
		corners[5] = lightMat * viewMatInv * glm::vec4(-xf, yf, -absFar, 1.f);
		corners[6] = lightMat * viewMatInv * glm::vec4(xf, -yf, -absFar, 1.f);
		corners[7] = lightMat * viewMatInv * glm::vec4(xf, yf, -absFar, 1.f);

		glm::vec3 minVal(std::numeric_limits<float>::max()), maxVal(std::numeric_limits<float>::min());
		for (size_t j = 0; j < 8; j++)
		{
			minVal = glm::min(corners[j], minVal);
			maxVal = glm::max(corners[j], maxVal);
		}
		m_UData->PrjMat = Frustum::OrthoMat(minVal.x, maxVal.x, minVal.y, maxVal.y, minVal.z, maxVal.z);
		m_UData->ViewMat = lightMat;
		m_UData->ShadowMapSize.x = m_frameBuff->width();
		m_UData->ShadowMapSize.y = m_frameBuff->height();

		m_UData->serialize();
		context->uploadData(m_UData->Block.getBuffer()->as<void*>(), m_UData->Block.getByteUsed(), m_UBuffer);
	}

	template<>
	bool Asset::is<ShadowMap>() const
	{
		if (is<LightTechBase>())
		{
			return as<LightTechBase>()->lightTech() == LightTech::ShadowMap;
		}
		return false;
	}
}