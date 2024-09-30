#ifndef __PIONNER_SCENE_3D_LIGHTING_TECH_H__
#define __PIONNER_SCENE_3D_LIGHTING_TECH_H__

#include "AABB.h"

namespace pio
{		
	class CameraNode;
	class DirectionalLightNode;
	class FrameBuffer;

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

		//AABB marked as frustum in lighting space
		AABB m_lightFrustums[CascadeNum];
		float m_cascadeEnds[CascadeNum + 1];

		Ref<FrameBuffer> m_frameBuff;
	};
}

#endif