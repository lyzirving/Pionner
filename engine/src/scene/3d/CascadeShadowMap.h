#ifndef __PIONNER_SCENE_3D_CASCADE_SHADOW_MAP_H__
#define __PIONNER_SCENE_3D_CASCADE_SHADOW_MAP_H__

#include "AABB.h"
#include "LightTechBase.h"

namespace pio
{
	class DirectionalLightNode;
	class FrameBuffer;
	class Camera;
	struct CascadeShadowMapUD;

	class CascadeShadowMap : public LightTechBase
	{
		PIO_LIGH_TECH_IMPL_TYPE(LightTech::CascadeShadowMap)
	public:
		CascadeShadowMap(Ref<RenderContext>& context);
		~CascadeShadowMap() = default;

		virtual bool bind(Ref<Shader>& shader) override;
		virtual bool bindUnimBlock(Ref<RenderContext>& context, Ref<Shader>& shader) override;
		virtual void unbindUnimBlock() override;
		virtual Ref<UniformBuffer>& UBuff() override { return m_UBuffer; }
		virtual Ref<FrameBuffer>& frameBuff() override { return m_frameBuff; }
		
		void update(Ref<RenderContext>& context, Ref<CameraNode>& camNode, Ref<DirectionalLightNode>& lightNode);			

	private:
		struct Corners
		{
			glm::vec3 NearLb{0.f};
			glm::vec3 NearLt{0.f};
			glm::vec3 NearRb{0.f};
			glm::vec3 NearRt{0.f};

			glm::vec3 FarLb{0.f};
			glm::vec3 FarLt{0.f};
			glm::vec3 FarRb{0.f};
			glm::vec3 FarRt{0.f};
		};

	private:
		//AABB marked as frustum in lighting space
		AABB m_lightFrustums[CASCADE_NUM];
		Corners m_lightFrustCorners[CASCADE_NUM];
		float m_cascadeEnds[CASCADE_NUM + 1];

		Ref<Camera> m_lightCam;
		Ref<FrameBuffer> m_frameBuff;	
		Ref<CascadeShadowMapUD> m_UData;
		Ref<UniformBuffer> m_UBuffer;
	};

	template<>
	bool Asset::is<CascadeShadowMap>() const;
}

#endif