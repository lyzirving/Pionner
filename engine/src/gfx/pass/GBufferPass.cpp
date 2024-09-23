#include "GBufferPass.h"

#include "GlobalSettings.h"

#include "asset/AssetMgr.h"

#include "gfx/rhi/Shader.h"
#include "gfx/rhi/FrameBuffer.h"
#include "gfx/rhi/UniformBuffer.h"
#include "gfx/renderer/RenderContext.h"

#include "gfx/resource/Mesh.h"
#include "gfx/resource/Material.h"
#include "gfx/resource/MeshRenderBuffer.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "GBufferPass"

namespace pio
{
	void GBufferPass::onAttach(Ref<RenderContext>& context)
	{
        auto colorSize = GlobalSettings::ColorResolution();
        auto depthSize = GlobalSettings::ShadowResolution();

        FrameBufferSpecific fboSpec;
        fboSpec.Name = "GeoBuffPass";
        fboSpec.Width = colorSize.x;
        fboSpec.Height = colorSize.y;
        PIO_FBO_ADD_USAGE(fboSpec.Usage, FrameBufferUsage_Color);

		TextureSpecificBuilder geoBuffBuilder;
		geoBuffBuilder.name(GpuAttr::UNI_GBUFFER_POS)
			.type(TextureType::TwoDimen)
			.format(TextureFormat::RGBA_HALF)
			.width(colorSize.x).height(colorSize.y)
            .texWrap(TextureWrap::ClampEdge, TextureWrap::ClampEdge)
            .texFilter(TextureFilterMin::Linear, TextureFilterMag::Linear);

        TextureSpecificBuilder normalBuffBuilder;
        normalBuffBuilder.name(GpuAttr::UNI_GBUFFER_NORMAL)
            .type(TextureType::TwoDimen)
            .format(TextureFormat::RGBA_HALF)
            .width(colorSize.x).height(colorSize.y)
            .texWrap(TextureWrap::ClampEdge, TextureWrap::ClampEdge)
            .texFilter(TextureFilterMin::Linear, TextureFilterMag::Linear);

        TextureSpecificBuilder albeoBuffBuilder;
        albeoBuffBuilder.name(GpuAttr::UNI_GBUFFER_ALBEDO)
            .type(TextureType::TwoDimen)
            .format(TextureFormat::RGBA_HALF)
            .width(colorSize.x).height(colorSize.y)
            .texWrap(TextureWrap::ClampEdge, TextureWrap::ClampEdge)
            .texFilter(TextureFilterMin::Linear, TextureFilterMag::Linear);

        TextureSpecificBuilder matBuffBuilder;
        matBuffBuilder.name(GpuAttr::UNI_GBUFFER_MATERIAL)
            .type(TextureType::TwoDimen)
            .format(TextureFormat::RGBA_HALF)
            .width(colorSize.x).height(colorSize.y)
            .texWrap(TextureWrap::ClampEdge, TextureWrap::ClampEdge)
            .texFilter(TextureFilterMin::Linear, TextureFilterMag::Linear);

        TextureSpecificBuilder emissionBuffBuilder;
        emissionBuffBuilder.name(GpuAttr::UNI_GBUFFER_EMISSION)
            .type(TextureType::TwoDimen)
            .format(TextureFormat::RGBA_HALF)
            .width(colorSize.x).height(colorSize.y)
            .texWrap(TextureWrap::ClampEdge, TextureWrap::ClampEdge)
            .texFilter(TextureFilterMin::Linear, TextureFilterMag::Linear);

        TextureSpecificBuilder depthBuilder;
        depthBuilder.name("GeoPass Depth Buffer")
            .type(TextureType::RenderBuffer)
            .format(TextureFormat::DEPTH_24_STENCIL_8)
            .width(depthSize.x).height(depthSize.y);

        fboSpec.ColorSpec.push_back(geoBuffBuilder.build());
        fboSpec.ColorSpec.push_back(normalBuffBuilder.build());
        fboSpec.ColorSpec.push_back(albeoBuffBuilder.build());
        fboSpec.ColorSpec.push_back(matBuffBuilder.build());
        fboSpec.ColorSpec.push_back(emissionBuffBuilder.build());
        fboSpec.DepthSpec = depthBuilder.build();

        m_frameBuff = FrameBuffer::Create(context, fboSpec);
        context->uploadData(m_frameBuff);

		m_attrs.setClear(Clear::Common())
			.setBlend(Blend::Disable())
			.setDepth(DepthTest::Common())
            .setCull(CullFace::Common())
			.setStencil(StencilTest::Disable());
	}

	void GBufferPass::onDetach(Ref<RenderContext>& context)
	{
        m_frameBuff.reset();
	}

	void GBufferPass::onExecute(Ref<RenderContext>& context, Ref<CameraNode>& camNode, Ref<RenderPass>& lastPass)
	{
        auto &renderingData = context->renderingData();
        std::vector<MeshRenderingItem> opaqueMeshItems = renderingData.OpaqueMeshItems;
        if (opaqueMeshItems.empty()) { return; }

        auto it = renderingData.UnimBuffSet.find(UBBinding_Camera);
        PIO_CHECK_RETURN(it != renderingData.UnimBuffSet.end(), "err! fail to find camera buff id");
        UUID32 camFilter = it->second;

        auto& fbo = m_frameBuff;
        auto& shader = context->shader(ShaderType::GBuffer);
        const auto& attr = m_attrs;        

		context->submitRC([&context, &fbo, &shader, &attr, camFilter, opaqueMeshItems]()
        {
            auto camBuff = AssetMgr::GetRuntimeAsset<UniformBuffer>(camFilter);
            PIO_CHECK_RETURN(camBuff, "err! invalid camera uniform buffer");

            context->onBeginFrameBuffer(fbo, attr);

            shader->bind();

            context->bindUnimBlock(shader, camBuff, GpuAttr::BINDING_CAM_BLOCK);
            camBuff->bind();

            for(const auto& item : opaqueMeshItems)
            {
                Ref<MeshRenderBuffer> meshBuff = AssetMgr::GetRuntimeAsset<MeshRenderBuffer>(item.RenderBuffFilter);
                Ref<Material> material = AssetMgr::GetRuntimeAsset<Material>(item.MaterialFilter);
                if(!meshBuff || !material)
                {
                    LOGW("warning! fail to find mesh render buffer or material in asset, it might be deleted");
                    continue;
                }
               
                meshBuff->bind(shader);
                material->bind(shader);
                context->drawTriangles(meshBuff);
            }

            camBuff->unbind();

            shader->unbind();                     
        });
	}

    template<>
    bool RenderPass::is<GBufferPass>() const { return type() == RenderPassType::GBuffer; }
}