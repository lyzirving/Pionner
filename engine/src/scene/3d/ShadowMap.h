#ifndef __PIONNER_SCENE_3D_SHADOW_MAP_H__
#define __PIONNER_SCENE_3D_SHADOW_MAP_H__

#include "LightTechBase.h"

namespace pio
{
	class DirectionalLightNode;	
	class FrameBuffer;
	class Camera;
	struct DirectionalLightShadowDataUD;

	class ShadowMap : public LightTechBase
	{
		PIO_LIGH_TECH_IMPL_TYPE(LightTech::ShadowMap)
	public:
		ShadowMap(Ref<RenderContext>& context);
		~ShadowMap() = default;

		virtual bool bind(Ref<Shader>& shader) override;
		virtual bool bindUnimBlock(Ref<RenderContext>& context, Ref<Shader>& shader) override;
		virtual void unbindUnimBlock() override;
		virtual Ref<UniformBuffer>& UBuff() override { return m_UBuffer; }
		virtual Ref<FrameBuffer>& frameBuff() override { return m_frameBuff; }

		void update(Ref<RenderContext>& context, Ref<CameraNode>& camNode, Ref<DirectionalLightNode>& lightNode);		

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