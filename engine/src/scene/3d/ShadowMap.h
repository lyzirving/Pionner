#ifndef __PIONNER_SCENE_3D_SHADOW_MAP_H__
#define __PIONNER_SCENE_3D_SHADOW_MAP_H__

#include "LightTechBase.h"

namespace pio
{
	class DirectionalLightNode;
	class UniformBuffer;
	class FrameBuffer;
	class Camera;
	struct DirectionalLightShadowDataUD;

	class ShadowMap : public LightTechBase
	{
		PIO_LIGH_TECH_IMPL_TYPE(LightTech::ShadowMap)
	public:
		ShadowMap(Ref<RenderContext>& context);
		~ShadowMap() = default;

		void update(Ref<RenderContext>& context, Ref<CameraNode>& camNode, Ref<DirectionalLightNode>& lightNode);
		Ref<FrameBuffer>& frameBuff() { return m_frameBuff; }
		Ref<UniformBuffer>& UBuff() { return m_UBuffer; }

	private:
		Ref<Camera> m_lightCam;
		Ref<FrameBuffer> m_frameBuff;
		Ref<DirectionalLightShadowDataUD> m_UData;
		Ref<UniformBuffer> m_UBuffer;
	};

	template<>
	bool Asset::is<ShadowMap>() const;
}

#endif