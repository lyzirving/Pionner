#include "ForwardPass.h"

#include "GlobalSettings.h"

#include "asset/material/Material.h"

#include "gfx/renderer/RenderContext.h"
#include "gfx/renderer/GBuffer.h"
#include "gfx/renderer/ShadowMap.h"
#include "gfx/renderer/PointLitShadowMap.h"
#include "gfx/renderer/IndirectLight.h"

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
        return;
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

            auto shader = ctx->FindShader(SHADER_SPEC_FORWARD, data.PermutationFlags);
            PIO_CHECK_RETURN(shader, "shader is invalid in ForwardPass");
            auto camUBuff = data.UBuffs[UBBinding_Camera];
            PIO_CHECK_RETURN(camUBuff, "camera is invalid in ForwardPass");
            auto dirLitBuff = data.UBuffs[UBBinding_DirectionalLit];
            PIO_CHECK_RETURN(dirLitBuff, "directional lit is invalid in ForwardPass");

            auto* shadowMap = data.Pipeline.DirLitShadowMap->As<ShadowMap>();
            PIO_CHECK_RETURN(shadowMap, "shadow map is invalid in ForwardPass");
            auto* depthBuff = shadowMap->DepthBuffer()->As<Texture2D>();
            PIO_CHECK_RETURN(depthBuff, "shadow map is invalid in ForwardPass");
            auto shadowUBuffer = shadowMap->GetUBuffer();
            PIO_CHECK_RETURN(shadowUBuffer, "shadow map's ubuffer is invalid in ForwardPass");

            auto pointLitBuff = data.UBuffs[UBBinding_PointLit];
            CubeMapArray* pointLitDepth{ nullptr };
            if(data.Pipeline.PointLitShadowMap) {
                pointLitDepth = data.Pipeline.PointLitShadowMap->DepthBuffer()->As<CubeMapArray>();
            }

            auto indirectLitRef = data.Pipeline.IndirectLight;
            IndirectLight* indirectLit = indirectLitRef ? indirectLitRef->As<IndirectLight>() : nullptr;

            ctx->OnBeginFrameBuffer(defferdBuff, self->GetRenderState());
            shader->Bind();

            camUBuff->BindBlock(ctx, shader);
            dirLitBuff->BindBlock(ctx, shader);            

            // dir lit shadow
            shadowUBuffer->BindBlock(ctx, shader);            

            if(pointLitBuff) { pointLitBuff->BindBlock(ctx, shader); }  
            if(indirectLit) { indirectLit->GetUBuffer()->BindBlock(ctx, shader); }

            for(size_t i = 0; i < data.TransparentMeshItems.size(); ++i)
            {
                if(indirectLit) { indirectLit->BindAt(shader); }

                depthBuff->BindAt(shader, GpuAttr::UNI_SHADOW_MAP);
                if(pointLitDepth) { 
                    pointLitDepth->BindAt(shader, GpuAttr::UNI_PTLIT_SHADOW_MAP); 
                }

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

            depthBuff->UnBind();
            shadowUBuffer->UnBind();

            if(pointLitBuff) { pointLitBuff->UnBind(); }
            if(pointLitDepth) { pointLitDepth->UnBind(); }
            if(indirectLit) 
            { 
                indirectLit->GetUBuffer()->UnBind();
                indirectLit->UnBindAt(); 
            }

            camUBuff->UnBind();
            shader->UnBind();
        });
    }
}