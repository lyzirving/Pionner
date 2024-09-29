#include "LightingTech.h"

#include "Camera.h"
#include "Frustum.h"

#include "scene/node/LightNode.h"
#include "scene/node/CameraNode.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "CascadeShadowMapping"

namespace pio
{
	CascadeShadowMapping::CascadeShadowMapping(Ref<DirectionalLightNode>& light) : m_light(light)
	{
		for (size_t i = 0; i < CascadeNum; i++)
		{
			m_lightFrustums[i] = CreateRef<OrthographicFrustum>();
		}
	}

	void CascadeShadowMapping::calcCascades(Ref<CameraNode>& camNode)
	{
		auto& cam = camNode->camera();

		const auto& lightDir = m_light->direction();
		auto lightMat = glm::lookAt(glm::vec3(0.f), lightDir, World::Up);

		glm::mat4 viewMatInv = cam->viewMat();
		viewMatInv = glm::inverse(viewMatInv);

		// frustum attrs in View space
		float l = cam->frustLeft();
		float r = cam->frustRight();
		float t = cam->frustTop();
		float b = cam->frustBottom();

		float interval = (cam->frustFar() - cam->frustNear()) / 3.f;
		m_cascadeEnds[0] = cam->frustNear();
		m_cascadeEnds[1] = m_cascadeEnds[0] + interval;
		m_cascadeEnds[2] = m_cascadeEnds[1] + interval;
		m_cascadeEnds[3] = cam->frustFar();

		// Calculate each frustum's 8 corners to get AABB
		// Firstly, transform the frustum in View space to World space
		// Then, transform the point into Lighting space
	}
}