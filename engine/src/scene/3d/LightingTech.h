#ifndef __PIONNER_SCENE_3D_LIGHTING_TECH_H__
#define __PIONNER_SCENE_3D_LIGHTING_TECH_H__

#include "Common.h"

namespace pio
{
	class Frustum;
	class CameraNode;
	class DirectionalLightNode;

	class CascadeShadowMapping
	{
	public:
		static constexpr uint8_t CascadeNum = 3;
	public:
		CascadeShadowMapping(Ref<DirectionalLightNode>& light);
		~CascadeShadowMapping() = default;

		void calcCascades(Ref<CameraNode>& camNode);

	private:
		Ref<DirectionalLightNode> m_light;

		Ref<Frustum> m_lightFrustums[CascadeNum];
		float m_cascadeEnds[CascadeNum + 1];
	};
}

#endif