#include "MainLitShadowCasterPass.h"

#include "gfx/rhi/Shader.h"
#include "gfx/rhi/FrameBuffer.h"
#include "gfx/rhi/UniformBuffer.h"
#include "gfx/rhi/UniformData.h"

#include "gfx/renderer/ShadowMap.h"
#include "gfx/renderer/MeshRenderBuffer.h"
#include "gfx/renderer/RenderContext.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "MainLitShadowCasterPass"

namespace pio
{
	MainLitShadowCasterPass::MainLitShadowCasterPass(const std::string& name, RenderPassEvent event) : RenderPass(name, event)
	{
		m_Attrs.SetClear(Clear::Common())
			.SetBlend(Blend::Disable())
			.SetDepth(DepthTest::Common())
			// According to Real-Time Rendering 7.4 ShadowMapping,
			// it's better to use front face depth(back face Culling) for sun shadow.		
			.SetCull(CullFace::Common())
			.SetStencil(StencilTest::Disable());
	}

	void MainLitShadowCasterPass::OnExecute(const Ref<RenderContext>& context, const Ref<CameraNode>& camera)
	{
		context->SubmitRC([weakSelf = WeakRef<MainLitShadowCasterPass>(Self<MainLitShadowCasterPass>()),
						  weakCtx = WeakRef<RenderContext>(context),
						  //RenderingData must be copied for cross thread work
						  data = context->GetRenderingData()]() mutable
		{
			auto self = weakSelf.lock();
			PIO_CHECK_RETURN(self, "self is invalid in MainLitShadowCasterPass");
			auto ctx = weakCtx.lock();
			PIO_CHECK_RETURN(ctx, "context is invalid in MainLitShadowCasterPass");
			auto frameBuff = data.Pipeline.DirLitShadowMap;			
			PIO_CHECK_RETURN(frameBuff, "frame buffer is invalid in MainLitShadowCasterPass");
			auto* shadowMap = frameBuff->As<ShadowMap>();
			PIO_CHECK_RETURN(shadowMap, "shadow map is invalid in MainLitShadowCasterPass");
			auto ubuffer = shadowMap->GetUBuffer();
			PIO_CHECK_RETURN(ubuffer, "ubuffer is invalid in MainLitShadowCasterPass");
			auto shader = ctx->FindShader(SHADER_SPEC_MAIN_LIT_SHADOW_CASTER);
			PIO_CHECK_RETURN(shader, "shader is invalid in MainLitShadowCasterPass");

			ctx->OnBeginFrameBuffer(frameBuff, self->GetRenderState());
			shader->Bind();
			ubuffer->BindBlock(ctx, shader);
			for(size_t i = 0; i < data.ShadowCasterItems.size(); ++i)
			{
				auto& item = data.ShadowCasterItems[i];
				for(auto& buff : item.UniformBuffers)
					buff->BindBlock(ctx, shader);

				ctx->DrawTriangles(item.MeshBuff);
			}
			ubuffer->UnBind();
			shader->UnBind();
		});
	}
}