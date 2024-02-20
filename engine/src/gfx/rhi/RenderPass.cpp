#include "RenderPass.h"

#include "RenderAPI.h"
#include "UniformBufferSet.h"

#include "gfx/renderer/Renderer.h"
#include "gfx/struct/MeshUtil.h"

#include "platform/opengl/GLRenderPass.h"
#include "asset/AssetsManager.h"
#include "scene/Light.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "RenderPass"

namespace pio
{
	Ref<RenderPass> RenderPass::Create(const RenderPassSpecification &spec)
	{
		switch (RenderAPI::CurrentType())
		{
			case RenderAPI::Type::OpenGL:
				return CreateRef<GLRenderPass>(spec);
			default:
				return Ref<RenderPass>();
		}
	}

	void RenderPass::RenderLightingEffect_Deferred(const LightEnvironment &lightEnv, Ref<RenderPass> &GBufferPass, Ref<RenderPass> &distantLightShadowPass, Ref<RenderPass> &pointLightShadowPass, Ref<UniformBufferSet> &uniformBufferSet)
	{
		// Firstly, directional lighting effect for the whole screen
		RenderState dirLightEffectState;
		dirLightEffectState.DepthTest = DepthTest::Disable();
		dirLightEffectState.Cull = CullFace::Common();
		dirLightEffectState.Blend = Blend::Disable();
		dirLightEffectState.Stencil.Enable = false;
		Renderer::RenderDistantLightingQuad_Deferred(Renderer::GetConfig().FullScreenQuad, uniformBufferSet,
												     GBufferPass, distantLightShadowPass, dirLightEffectState);
		/*
		* ------------------------ Stencil Pass for point light effect -------------------------------
		* Guarantee that only the pixels in the stencil buffer covered by objects inside the bounding lighting sphere
		* will have a value greater than zero.
		*/
		RenderState lightVolumeState;		
		lightVolumeState.DepthTest = DepthTest(FuncAttr::Less, DepthTest::Mask::ReadOnly);// Only read depth 		
		lightVolumeState.Cull = CullFace::Disable();// Rendering both face of the light volume	
		lightVolumeState.Blend = Blend::Disable();
		lightVolumeState.Stencil.setFunc(StencilFunc{ FuncAttr::Always, 1, 0xff });// What we really care about the stencil operation
		// Referring to https://ogldev.org/www/tutorial37/tutorial37.html
		lightVolumeState.Stencil.setOp(StencilOp{ FuncAttr::Keep, FuncAttr::Decr, FuncAttr::Keep },
									   StencilOp{ FuncAttr::Keep, FuncAttr::Incr, FuncAttr::Keep });
		for (uint32_t i = 0; i < lightEnv.PointLightData.LightCount; i++)
		{
			const PointLight &pointLight = lightEnv.PointLightData.Lights[i];
			Ref<StaticMesh> mesh = AssetsManager::GetRuntimeAsset<StaticMesh>(pointLight.Volume);
			glm::mat4 transform = glm::translate(glm::mat4(1.f), pointLight.Position);
			Renderer::RenderLightVolume(mesh->getHandle(), 0, transform, uniformBufferSet, lightVolumeState);
		}
		// -------------------------------------------------------------------------------------------

		// Point light's lighting pass After Stencil Pass
		// Configure the stencil test to pass only when the stencil value of the pixel is different from zero.	
		// Lighting the area that is occupied by light volume	
		RenderState lightEffectState;
		lightEffectState.DepthTest = DepthTest::Disable();// For performance consideration
		lightEffectState.Cull = CullFace::Common();
		lightEffectState.Blend = Blend(BlendFactor::One, BlendFactor::One, BlendEquation::Add);
		lightEffectState.Stencil.setFunc(StencilFunc{ FuncAttr::Notequal, 0, 0xff });
		lightEffectState.Stencil.setOp(StencilOp{ FuncAttr::Keep });
		Renderer::RenderPointLightQuad_Deferred(Renderer::GetConfig().FullScreenQuad, uniformBufferSet,
											    GBufferPass, pointLightShadowPass, lightEffectState);
	}

	void RenderPass::RenderSprites(std::map<MeshKey, SpriteCommand> &cmd)
	{
		for (auto &it : cmd)
		{			
			SpriteCommand &d = it.second;
			Renderer::RenderSprite(d.QuadMesh, d.Texture, d.State);
		}
	}

	void RenderPass::Postprocessing(AssetHandle &quadMesh, Ref<Texture2D> &composite)
	{
		RenderState state;
		state.Blend = Blend::Disable();
		state.DepthTest = DepthTest::Disable();
		state.Cull = CullFace::Common();
		state.Stencil.Enable = false;
		Renderer::Postprocessing(quadMesh, composite, state);
	}
}