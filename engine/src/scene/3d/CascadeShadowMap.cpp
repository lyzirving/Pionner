#include "CascadeShadowMap.h"

#include "Camera.h"
#include "Frustum.h"
#include "ShadowMapFrameBuffer.h"

#include "gfx/renderer/RenderContext.h"
#include "gfx/resource/LightData.h"

#include "scene/Components.h"
#include "scene/node/LightNode.h"
#include "scene/node/CameraNode.h"
#include "scene/node/GizmoNode.h"

#include <limits>

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "CascadeShadowMap"

namespace pio
{
	CascadeShadowMap::CascadeShadowMap(Ref<RenderContext>& context) : LightTechBase()
	{
		m_frameBuff = CreateRef<CascadeShadowFrameBuffer>(context);						

		m_UData = CreateRef<CascadeShadowMapUD>();
		m_UBuffer = UniformBuffer::Create(context, m_UData->Block.getByteUsed(), UBBinding_DirectionalLightShadow, BufferUsage::Dynamic);
	}

	void CascadeShadowMap::update(Ref<RenderContext>& context, Ref<CameraNode>& camNode, Ref<DirectionalLightNode>& lightNode)
	{
		auto& cam = camNode->camera();
		const auto& lightDir = lightNode->direction();

		auto lightMat = glm::lookAt(glm::vec3(0.f), lightDir, World::Up);
		glm::mat4 viewMatInv = cam->viewMat();
		viewMatInv = glm::inverse(viewMatInv);

		float aspect = cam->aspect();// width / height
		float fov = cam->fov();		
		float interval = (cam->frustFar() - cam->frustNear()) / float(CASCADE_NUM);
		m_cascadeEnds[0] = cam->frustNear();
		m_cascadeEnds[1] = m_cascadeEnds[0] + interval;
		m_cascadeEnds[2] = m_cascadeEnds[1] + interval;
		m_cascadeEnds[3] = cam->frustFar();

		float tanHalfFov = std::tan(glm::radians(fov * 0.5f));		

		// Calculate each frustum's 8 corners to get AABB
		// Firstly, transform the frustum in View space to World space
		// Then, transform the point into Lighting space
		for (size_t i = 0; i < CASCADE_NUM; i++)
		{			
			float xn = m_cascadeEnds[i] * tanHalfFov * aspect; 
			float yn = m_cascadeEnds[i] * tanHalfFov;

			m_lightFrustCorners[i].NearLb = viewMatInv * glm::vec4(-xn, -yn, -m_cascadeEnds[i], 1.f);//near left-bottom
			m_lightFrustCorners[i].NearLt = viewMatInv * glm::vec4(-xn, yn, -m_cascadeEnds[i], 1.f); //near left-top
			m_lightFrustCorners[i].NearRb = viewMatInv * glm::vec4(xn, -yn, -m_cascadeEnds[i], 1.f); //near right-bottom
			m_lightFrustCorners[i].NearRt = viewMatInv * glm::vec4(xn, yn, -m_cascadeEnds[i], 1.f);  //near right-top

			float xf = m_cascadeEnds[i + 1] * tanHalfFov * aspect;
			float yf = m_cascadeEnds[i + 1] * tanHalfFov;

			m_lightFrustCorners[i].FarLb = viewMatInv * glm::vec4(-xf, -yf, -m_cascadeEnds[i + 1], 1.f);
			m_lightFrustCorners[i].FarLt = viewMatInv * glm::vec4(-xf, yf, -m_cascadeEnds[i + 1], 1.f);
			m_lightFrustCorners[i].FarRb = viewMatInv * glm::vec4(xf, -yf, -m_cascadeEnds[i + 1], 1.f);
			m_lightFrustCorners[i].FarRt = viewMatInv * glm::vec4(xf, yf, -m_cascadeEnds[i + 1], 1.f);

			glm::vec4 corners[8];
			corners[0] = lightMat * glm::vec4(m_lightFrustCorners[i].NearLb, 1.f);
			corners[1] = lightMat * glm::vec4(m_lightFrustCorners[i].NearLt, 1.f);
			corners[2] = lightMat * glm::vec4(m_lightFrustCorners[i].NearRb, 1.f);
			corners[3] = lightMat * glm::vec4(m_lightFrustCorners[i].NearRt, 1.f);

			corners[4] = lightMat * glm::vec4(m_lightFrustCorners[i].FarLb, 1.f);
			corners[5] = lightMat * glm::vec4(m_lightFrustCorners[i].FarLt, 1.f);
			corners[6] = lightMat * glm::vec4(m_lightFrustCorners[i].FarRb, 1.f);
			corners[7] = lightMat * glm::vec4(m_lightFrustCorners[i].FarRt, 1.f);

			glm::vec4 minVal(std::numeric_limits<float>::max()), maxVal(std::numeric_limits<float>::min());
			for (size_t j = 0; j < 8; j++)
			{
				minVal = glm::min(corners[j], minVal);
				maxVal = glm::max(corners[j], maxVal);
			}
			m_lightFrustums[i].setMax(maxVal);
			m_lightFrustums[i].setMin(minVal);
			m_UData->PrjMats[i] = Frustum::OrthoMat(minVal.x, maxVal.x, minVal.y, maxVal.y, maxVal.z, minVal.z);
		}

		m_UData->ViewMat = lightMat;
		m_UData->CascadeNum = CASCADE_NUM;
		m_UData->serialize();
		context->uploadData(m_UData->Block.getBuffer()->as<void*>(), m_UData->Block.getByteUsed(), m_UBuffer);
	}

	template<>
	bool Asset::is<CascadeShadowMap>() const
	{
		if (is<LightTechBase>())
		{
			return as<LightTechBase>()->lightTech() == LightTech::CascadeShadowMap;
		}
		return false;
	}
}