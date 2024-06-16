#include "RenderPass.h"

#include "RenderAPI.h"
#include "UniformBufferSet.h"

#include "gfx/renderer/Renderer.h"
#include "gfx/struct/MeshBuilder.h"

#include "platform/opengl/GLRenderPass.h"
#include "asset/AssetsManager.h"

#include "scene/Skybox.h"
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

	void RenderPass::RenderSprites(std::map<MeshKey, SpriteCommand> &cmd)
	{
		for (auto &it : cmd)
		{			
			SpriteCommand &d = it.second;			
			Renderer::RenderSprite(d.QuadMesh, d.Texture, d.State);
		}
	}

	void RenderPass::Postprocessing(const AssetHandle &quadMesh, Ref<Texture2D> &composite)
	{
		RenderState state;
		state.Blend = Blend::Disable();
		state.DepthTest = DepthTest::Disable();
		state.Cull = CullFace::Common();
		state.Stencil.Enable = false;
		Renderer::Postprocessing(quadMesh, composite, state);
	}
}