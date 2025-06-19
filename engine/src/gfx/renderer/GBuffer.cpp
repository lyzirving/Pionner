#include "GBuffer.h"
#include "RenderContext.h"

#include "GlobalSettings.h"

#include "gfx/rhi/Shader.h"
#include "gfx/rhi/Texture.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "GBuffer"

namespace pio
{
    GBuffer::GBuffer(const Ref<RenderContext>& context, const std::string& name)
        : FrameBufferProxy(context, name)
    {
        auto colorSize = GlobalSettings::ColorResolution();
        auto depthSize = GlobalSettings::ShadowResolution();

        FrameBufferSpecific fboSpec;
        fboSpec.Name = name;
        fboSpec.Width = colorSize.x;
        fboSpec.Height = colorSize.y;
        PIO_FBO_ADD_USAGE(fboSpec.Usage, FrameBufferUsage_Color);

        TextureSpecificBuilder geoBuffBuilder;
        geoBuffBuilder.Name(GpuAttr::UNI_GBUFFER_POS)
            .Type(TextureType::TwoDimen)
            .Format(TextureFormat::RGBA_HALF)
            .Width(colorSize.x).Height(colorSize.y)
            .TexWrap(TextureWrap::ClampEdge, TextureWrap::ClampEdge)
            .TexFilter(TextureFilterMin::Linear, TextureFilterMag::Linear);

        TextureSpecificBuilder normalBuffBuilder;
        normalBuffBuilder.Name(GpuAttr::UNI_GBUFFER_NORMAL)
            .Type(TextureType::TwoDimen)
            .Format(TextureFormat::RGBA_HALF)
            .Width(colorSize.x).Height(colorSize.y)
            .TexWrap(TextureWrap::ClampEdge, TextureWrap::ClampEdge)
            .TexFilter(TextureFilterMin::Linear, TextureFilterMag::Linear);

        TextureSpecificBuilder albeoBuffBuilder;
        albeoBuffBuilder.Name(GpuAttr::UNI_GBUFFER_ALBEDO)
            .Type(TextureType::TwoDimen)
            .Format(TextureFormat::RGBA_HALF)
            .Width(colorSize.x).Height(colorSize.y)
            .TexWrap(TextureWrap::ClampEdge, TextureWrap::ClampEdge)
            .TexFilter(TextureFilterMin::Linear, TextureFilterMag::Linear);

        TextureSpecificBuilder matBuffBuilder;
        matBuffBuilder.Name(GpuAttr::UNI_GBUFFER_MATERIAL)
            .Type(TextureType::TwoDimen)
            .Format(TextureFormat::RGBA_HALF)
            .Width(colorSize.x).Height(colorSize.y)
            .TexWrap(TextureWrap::ClampEdge, TextureWrap::ClampEdge)
            .TexFilter(TextureFilterMin::Linear, TextureFilterMag::Linear);

        TextureSpecificBuilder matSubBuffBuilder;
        matSubBuffBuilder.Name(GpuAttr::UNI_GBUFFER_MATERIAL_SUB)
            .Type(TextureType::TwoDimen)
            .Format(TextureFormat::RGBA_HALF)
            .Width(colorSize.x).Height(colorSize.y)
            .TexWrap(TextureWrap::ClampEdge, TextureWrap::ClampEdge)
            .TexFilter(TextureFilterMin::Linear, TextureFilterMag::Linear);

        TextureSpecificBuilder depthBuilder;
        depthBuilder.Name("GBuffer Depth Buffer")
            .Type(TextureType::RenderBuffer)
            .Format(TextureFormat::DEPTH_24_STENCIL_8)
            .Width(depthSize.x).Height(depthSize.y);

        fboSpec.ColorSpec.push_back(geoBuffBuilder.Build());
        fboSpec.ColorSpec.push_back(normalBuffBuilder.Build());
        fboSpec.ColorSpec.push_back(albeoBuffBuilder.Build());
        fboSpec.ColorSpec.push_back(matBuffBuilder.Build());
        fboSpec.ColorSpec.push_back(matSubBuffBuilder.Build());
        fboSpec.DepthSpec.push_back(depthBuilder.Build());

        m_FrameBuff = FrameBuffer::Create(context, fboSpec);
        context->UploadData(m_FrameBuff);
    }

    void GBuffer::BindContent(const Ref<Shader>& shader)
    {
        GetGeometryBuffer()->BindAt(shader, GpuAttr::UNI_GBUFFER_POS);
        GetNormalBuffer()->BindAt(shader, GpuAttr::UNI_GBUFFER_NORMAL);
        GetAlbedoBuffer()->BindAt(shader, GpuAttr::UNI_GBUFFER_ALBEDO);
        GetMaterialBuffer()->BindAt(shader, GpuAttr::UNI_GBUFFER_MATERIAL);
        GetMaterialSubBuffer()->BindAt(shader, GpuAttr::UNI_GBUFFER_MATERIAL_SUB);
    }

    void GBuffer::UnBindContent()
    {
        GetGeometryBuffer()->UnBind();
        GetNormalBuffer()->UnBind();
        GetAlbedoBuffer()->UnBind();
        GetMaterialBuffer()->UnBind();
        GetMaterialSubBuffer()->UnBind();
    }
}