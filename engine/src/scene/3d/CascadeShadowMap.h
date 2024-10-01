#ifndef __PIONNER_SCENE_3D_CASCADE_SHADOW_MAP_H__
#define __PIONNER_SCENE_3D_CASCADE_SHADOW_MAP_H__

#include "AABB.h"
#include "LightTechBase.h"

namespace pio
{	
	class FrameBuffer;
	class DirectionalLightNode;

	class CascadeShadowMap : public LightTechBase
	{
		PIO_LIGH_TECH_IMPL_TYPE(LightTech::CascadeShadowMap)
	public:
		static constexpr uint8_t CascadeNum = 3;
	public:
		CascadeShadowMap();
		~CascadeShadowMap() = default;
		
		void update(Ref<RenderContext>& context, Ref<CameraNode>& camNode, Ref<DirectionalLightNode>& lightNode);

	private:
		//AABB marked as frustum in lighting space
		AABB m_lightFrustums[CascadeNum];
		float m_cascadeEnds[CascadeNum + 1];

		Ref<FrameBuffer> m_frameBuff;
	};

	template<>
	bool Asset::is<CascadeShadowMap>() const;
}

#endif