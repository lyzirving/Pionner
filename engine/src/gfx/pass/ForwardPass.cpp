#include "ForwardPass.h"

#include "GlobalSettings.h"

#include "asset/material/Material.h"

#include "gfx/renderer/RenderContext.h"
#include "gfx/renderer/GBuffer.h"
#include "gfx/renderer/ShadowMap.h"
#include "gfx/renderer/PointLitShadowMap.h"

#include "gfx/rhi/Shader.h"
#include "gfx/rhi/FrameBuffer.h"
#include "gfx/rhi/UniformBuffer.h"
#include "gfx/rhi/Texture.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "ForwardPass"

namespace pio
{
    void ForwardPass::OnAttach(const Ref<RenderContext>& context)
    {
        m_Attrs.SetClear(Clear::Common())
            .SetCull(CullFace::Common())
            .SetBlend(Blend::Common())
            .SetDepth(DepthTest::Common())            
            .SetStencil(StencilTest::Disable());
    }

    void ForwardPass::OnDetach(const Ref<RenderContext>& context)
    {       
    }

	void ForwardPass::OnExecute(const Ref<RenderContext>& context, const Ref<CameraNode>& camera)
    {
        if(context->GetRenderingData().TransparentMeshItems.empty())
            return;

        context->SubmitRC([weakSelf = WeakRef<ForwardPass>(Self<ForwardPass>()), 
                          weakCtx = WeakRef<RenderContext>(context),
                          //RenderingData must be copied for cross thread work
                          data = context->GetRenderingData()]() mutable
        {
            auto self = weakSelf.lock();
            PIO_CHECK_RETURN(self, "self is invalid in ForwardPass");
            auto ctx = weakCtx.lock();
            PIO_CHECK_RETURN(self, "context is invalid in ForwardPass");
            GBuffer* gBuff = data.Pipeline.GBuffer.use_count() != 0 ? data.Pipeline.GBuffer->As<GBuffer>() : nullptr;
            PIO_CHECK_RETURN(gBuff, "GBuffer is invalid in ForwardPass");
            auto defferdBuff = data.Pipeline.DefferedBuffer;
            PIO_CHECK_RETURN(defferdBuff, "DefferedBuffer is invalid in ForwardPass");
            if(!ctx->CopyFrameBufferDepth(data.Pipeline.GBuffer, defferdBuff))
                return;

            auto shader = ctx->FindShader(SHADER_SPEC_FORWARD);
            PIO_CHECK_RETURN(shader, "shader is invalid in ForwardPass");
            auto camUBuff = data.UBuffs[UBBinding_Camera];
            PIO_CHECK_RETURN(camUBuff, "camera is invalid in ForwardPass");
            auto dirLitBuff = data.UBuffs[UBBinding_DirectionalLit];
            PIO_CHECK_RETURN(dirLitBuff, "directional lit is invalid in ForwardPass");

            ctx->OnBeginFrameBuffer(defferdBuff, self->GetRenderState());
            shader->Bind();

            camUBuff->BindBlock(ctx, shader);
            dirLitBuff->BindBlock(ctx, shader);

            for(size_t i = 0; i < data.TransparentMeshItems.size(); ++i)
            {
                auto& item = data.TransparentMeshItems[i];

                item.Material->Bind(shader);

                for(auto& buff : item.UniformBuffers)
                    buff->BindBlock(ctx, shader);

                ctx->DrawTriangles(item.MeshBuff);

                item.Material->UnBind();

                for(auto& buff : item.UniformBuffers)
                    buff->UnBind();

                shader->FreeSamplerUnit();
            }

            camUBuff->UnBind();
            shader->UnBind();
        });
    }
}