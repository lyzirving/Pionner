#include "DefferedPass.h"

#include "GlobalSettings.h"

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
#define LOCAL_TAG "DefferedPass"

namespace pio
{
    void DefferedPass::OnAttach(const Ref<RenderContext>& context)
    {
        StencilTest stencil = StencilTest::Common();
        stencil.SetMask(0xff);
        stencil.SetOp(StencilOp(FuncAttr::Keep, FuncAttr::Keep, FuncAttr::Keep));
        stencil.SetFunc(StencilFunc(FuncAttr::Equal, GpuAttr::Stencil::VALID_GEOMETRY, 0xff));

        m_Attrs.SetClear(Clear::Common())
            .SetCull(CullFace::Common())
            .SetBlend(Blend::Disable())
            .SetDepth(DepthTest::Disable())            
            .SetStencil(stencil);

        auto colorSize = GlobalSettings::ColorResolution();
        auto depthSize = GlobalSettings::ShadowResolution();

        FrameBufferSpecific fboSpec;
        fboSpec.Name = "DefferedPass";
        fboSpec.Width = colorSize.x;
        fboSpec.Height = colorSize.y;
        PIO_FBO_ADD_USAGE(fboSpec.Usage, FrameBufferUsage_Color);

        TextureSpecificBuilder colorBufferSpec;
        colorBufferSpec.Name(GpuAttr::UNI_COLOR_BUFFER)
            .Type(TextureType::TwoDimen)
            .Format(TextureFormat::RGBA_HALF)
            .Width(colorSize.x).Height(colorSize.y)
            .TexWrap(TextureWrap::ClampEdge, TextureWrap::ClampEdge)
            .TexFilter(TextureFilterMin::Linear, TextureFilterMag::Linear);        

        TextureSpecificBuilder depthBuilder;
        depthBuilder.Name("Defferred Depth Buffer")
            .Type(TextureType::RenderBuffer)
            .Format(TextureFormat::DEPTH_24_STENCIL_8)
            .Width(depthSize.x).Height(depthSize.y);

        fboSpec.ColorSpec.push_back(colorBufferSpec.Build());
        fboSpec.DepthSpec.push_back(depthBuilder.Build());

        m_FrameBuff = FrameBuffer::Create(context, fboSpec);
        context->UploadData(m_FrameBuff);
    }

    void DefferedPass::OnDetach(const Ref<RenderContext>& context)
    {
        m_FrameBuff.reset();
    }

	void DefferedPass::OnExecute(const Ref<RenderContext>& context, const Ref<CameraNode>& camera)
    {
        context->GetRenderingData().Pipeline.DefferedBuffer = m_FrameBuff;
        context->SubmitRC([weakSelf = WeakRef<DefferedPass>(Self<DefferedPass>()), weakCtx = WeakRef<RenderContext>(context),
                          //RenderingData must be copied for cross thread work
                          data = context->GetRenderingData()]() mutable
        {
            auto self = weakSelf.lock();
            PIO_CHECK_RETURN(self, "self is invalid in DefferedPass");
            auto ctx = weakCtx.lock();
            PIO_CHECK_RETURN(self, "context is invalid in DefferedPass");
            auto camUBuff = data.UBuffs[UBBinding_Camera];
            PIO_CHECK_RETURN(camUBuff, "camera is invalid in DefferedPass");
            auto dirLitBuff = data.UBuffs[UBBinding_DirectionalLit];
            PIO_CHECK_RETURN(dirLitBuff, "directional lit is invalid in DefferedPass");
            GBuffer* gBuff = data.Pipeline.GBuffer.use_count() != 0 ? data.Pipeline.GBuffer->As<GBuffer>() : nullptr;
            PIO_CHECK_RETURN(gBuff, "GBuffer is invalid in DefferedPass");
            auto shader = ctx->FindShader(SHADER_SPEC_DEFERRED, data.PermutationFlags);
            PIO_CHECK_RETURN(shader, "shader is invalid in DefferedPass");
            auto* shadowMap = data.Pipeline.DirLitShadowMap->As<ShadowMap>();
            PIO_CHECK_RETURN(shadowMap, "shadow map is invalid in DefferedPass");
            auto* depthBuff = shadowMap->DepthBuffer()->As<Texture2D>();
            PIO_CHECK_RETURN(depthBuff, "shadow map is invalid in DefferedPass");
            auto shadowUBuffer = shadowMap->GetUBuffer();
            PIO_CHECK_RETURN(shadowUBuffer, "shadow map's ubuffer is invalid in DefferedPass");
            auto pointLitBuff = data.UBuffs[UBBinding_PointLit];
			CubeMapArray* pointLitDepth{ nullptr };
            if(data.Pipeline.PointLitShadowMap)
            {
                pointLitDepth = data.Pipeline.PointLitShadowMap->DepthBuffer()->As<CubeMapArray>();
            }
            auto indirectLitRef = data.Pipeline.IndirectLight;
            IndirectLight* indirectLit = indirectLitRef ? indirectLitRef->As<IndirectLight>() : nullptr;            

            ctx->OnBeginFrameBuffer(self->GetFrameBuffer(), self->GetRenderState());

            if(!ctx->CopyFrameBufferStencil(data.Pipeline.GBuffer, self->GetFrameBuffer()))
                return;

            shader->Bind();
            
            camUBuff->BindBlock(ctx, shader);
            dirLitBuff->BindBlock(ctx, shader); 
            shadowUBuffer->BindBlock(ctx, shader);

            if(indirectLit) { indirectLit->BindAt(ctx, shader); }
            if(pointLitBuff) { pointLitBuff->BindBlock(ctx, shader); }
            if(pointLitDepth) { pointLitDepth->BindAt(shader, GpuAttr::UNI_PTLIT_SHADOW_MAP); }

            gBuff->BindContent(shader);
            depthBuff->BindAt(shader, GpuAttr::UNI_SHADOW_MAP);                       

            ctx->DrawTriangles(ctx->GetScreenMeshBuff());

            if(indirectLit) { indirectLit->UnBindAt(); }
            if(pointLitBuff) { pointLitBuff->UnBind(); }
            if(pointLitDepth) { pointLitDepth->UnBind(); }
            depthBuff->UnBind();
            shadowUBuffer->UnBind();            
            dirLitBuff->UnBind();
            camUBuff->UnBind();            
            gBuff->UnBindContent();
            shader->UnBind();
        });
    }
}