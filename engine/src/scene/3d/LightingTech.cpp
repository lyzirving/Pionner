#include "LightingTech.h"

#include "Camera.h"
#include "Frustum.h"

#include "scene/node/LightNode.h"
#include "scene/node/CameraNode.h"

#include "gfx/rhi/FrameBuffer.h"

#include <limits>

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "CascadeShadowMapping"

namespace pio
{
	CascadeShadowMapping::CascadeShadowMapping(Ref<DirectionalLightNode>& light) : m_light(light)
	{		
	}

	void CascadeShadowMapping::calcCascades(Ref<CameraNode>& camNode)
	{
		auto& cam = camNode->camera();

		const auto& lightDir = m_light->direction();
		auto lightMat = glm::lookAt(glm::vec3(0.f), lightDir, World::Up);

		glm::mat4 viewMatInv = cam->viewMat();
		viewMatInv = glm::inverse(viewMatInv);

		float aspect = cam->aspect();// width / height
		float vFov = cam->fov();
		float hFov = aspect * vFov;		
		float interval = (cam->frustFar() - cam->frustNear()) / float(CascadeNum);
		m_cascadeEnds[0] = cam->frustNear();
		m_cascadeEnds[1] = m_cascadeEnds[0] + interval;
		m_cascadeEnds[2] = m_cascadeEnds[1] + interval;
		m_cascadeEnds[3] = cam->frustFar();

		float tanHalfHFov = std::tan(glm::radians(hFov * 0.5f));
		float tanHalfVFov = std::tan(glm::radians(vFov * 0.5f));

		// Calculate each frustum's 8 corners to get AABB
		// Firstly, transform the frustum in View space to World space
		// Then, transform the point into Lighting space
		for(size_t i = 0; i < CascadeNum; i++)
		{
			glm::vec4 corners[8];
			float xn = m_cascadeEnds[i] * tanHalfHFov;
			float yn = m_cascadeEnds[i] * tanHalfVFov;

			corners[0] = glm::vec4(-xn, -yn, m_cascadeEnds[i], 1.f);//near left-bottom
			corners[1] = glm::vec4(-xn, yn, m_cascadeEnds[i], 1.f); //near left-top
			corners[2] = glm::vec4(xn, -yn, m_cascadeEnds[i], 1.f); //near right-bottom
			corners[3] = glm::vec4(xn, yn, m_cascadeEnds[i], 1.f);  //near right-top

			float xf = m_cascadeEnds[i + 1] * tanHalfHFov;
			float yf = m_cascadeEnds[i + 1] * tanHalfVFov;

			corners[4] = glm::vec4(-xf, -yf, m_cascadeEnds[i + 1], 1.f);
			corners[5] = glm::vec4(-xf, yf, m_cascadeEnds[i + 1], 1.f);
			corners[6] = glm::vec4(xf, -yf, m_cascadeEnds[i + 1], 1.f);
			corners[7] = glm::vec4(xf, yf, m_cascadeEnds[i + 1], 1.f);

			corners[0] = lightMat * viewMatInv * corners[0];
			corners[1] = lightMat * viewMatInv * corners[1];
			corners[2] = lightMat * viewMatInv * corners[2];
			corners[3] = lightMat * viewMatInv * corners[3];

			corners[4] = lightMat * viewMatInv * corners[4];
			corners[5] = lightMat * viewMatInv * corners[5];
			corners[6] = lightMat * viewMatInv * corners[6];
			corners[7] = lightMat * viewMatInv * corners[7];
			
			glm::vec4 minVal(std::numeric_limits<float>::max()), maxVal(std::numeric_limits<float>::min());
			for(size_t j = 0; j < 8; j++)
			{
				minVal = glm::min(corners[j], minVal);
				maxVal = glm::max(corners[j], maxVal);
			}
			m_lightFrustums[i].setMax(maxVal);
			m_lightFrustums[i].setMin(minVal);
		}
	}
}