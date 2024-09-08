#include "GBufferPass.h"

#include "GlobalSettings.h"

#include "asset/AssetMgr.h"

#include "gfx/rhi/Shader.h"
#include "gfx/rhi/FrameBuffer.h"
#include "gfx/rhi/UniformBuffer.h"
#include "gfx/renderer/RenderContext.h"
#include "gfx/resource/MeshRenderBuffer.h"

#include "scene/resources/Mesh.h"
#include "scene/resources/Material.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "GBufferPass"

namespace pio
{
	void GBufferPass::onAttach(Ref<RenderContext>& context)
	{
        uint32_t colorW = GlobalSettings::ColorResolution(GlobalSettings::ColorResoSetting);
        uint32_t colorH = colorW / GlobalSettings::AspectRatio(GlobalSettings::AspectSetting);

        uint32_t depthW = GlobalSettings::ShadowResolution(GlobalSettings::ShadowResoSetting);
        uint32_t depthH = depthW / GlobalSettings::AspectRatio(GlobalSettings::AspectSetting);

        FrameBufferSpecific fboSpec;
        fboSpec.Name = "GeoBuffPass";
        fboSpec.Width = colorW;
        fboSpec.Height = colorH;
        PIO_FBO_ADD_USAGE(fboSpec.Usage, FrameBufferUsage_Color);

		TextureSpecificBuilder geoBuffBuilder;
		geoBuffBuilder.name(GpuAttr::UNI_GBUFFER_POS)
			.type(TextureType::TwoDimen)
			.format(TextureFormat::RGBA_HALF)
			.width(colorW).height(colorH)
            .texWrap(TextureWrap::ClampEdge, TextureWrap::ClampEdge)
            .texFilter(TextureFilterMin::Nearest, TextureFilterMag::Nearest);		

        TextureSpecificBuilder normalBuffBuilder;
        normalBuffBuilder.name(GpuAttr::UNI_GBUFFER_NORMAL)
            .type(TextureType::TwoDimen)
            .format(TextureFormat::RGBA_HALF)
            .width(colorW).height(colorH)
            .texWrap(TextureWrap::ClampEdge, TextureWrap::ClampEdge)
            .texFilter(TextureFilterMin::Nearest, TextureFilterMag::Nearest);

        TextureSpecificBuilder albeoBuffBuilder;
        albeoBuffBuilder.name(GpuAttr::UNI_GBUFFER_ALBEDO)
            .type(TextureType::TwoDimen)
            .format(TextureFormat::RGBA_HALF)
            .width(colorW).height(colorH)
            .texWrap(TextureWrap::ClampEdge, TextureWrap::ClampEdge)
            .texFilter(TextureFilterMin::Nearest, TextureFilterMag::Nearest);

        TextureSpecificBuilder matBuffBuilder;
        matBuffBuilder.name(GpuAttr::UNI_GBUFFER_MATERIAL)
            .type(TextureType::TwoDimen)
            .format(TextureFormat::RGBA_HALF)
            .width(colorW).height(colorH)
            .texWrap(TextureWrap::ClampEdge, TextureWrap::ClampEdge)
            .texFilter(TextureFilterMin::Nearest, TextureFilterMag::Nearest);

        TextureSpecificBuilder emissionBuffBuilder;
        emissionBuffBuilder.name(GpuAttr::UNI_GBUFFER_EMISSION)
            .type(TextureType::TwoDimen)
            .format(TextureFormat::RGBA_HALF)
            .width(colorW).height(colorH)
            .texWrap(TextureWrap::ClampEdge, TextureWrap::ClampEdge)
            .texFilter(TextureFilterMin::Nearest, TextureFilterMag::Nearest);

        TextureSpecificBuilder depthBuilder;
        depthBuilder.name("GeoPass Depth Buffer")
            .type(TextureType::RenderBuffer)
            .format(TextureFormat::DEPTH_24_STENCIL_8)
            .width(depthW).height(depthH);

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

	void GBufferPass::onExecute(Ref<RenderContext>& context, Ref<RenderPass>& lastPass)
	{
        auto &renderingData = context->renderingData();
        std::vector<MeshRenderingItem> opaqueMeshItems = std::move(renderingData.OpaqueMeshItems);
        if (opaqueMeshItems.empty())
        {
            return;
        }
        auto it = renderingData.UnimBuffSet.find(UBBinding_Camera);
        if(it == renderingData.UnimBuffSet.end())
        {
            LOGE("err! fail to find camera buff id");
            return;
        }
        UUID32 camFilter = it->second;

        auto& fbo = m_frameBuff;
        auto& shader = context->shader(ShaderType::GBuffer);
        const auto& attr = m_attrs;        

		context->submitRC([&context, &fbo, &shader, &attr, camFilter, opaqueMeshItems]()
        {
            auto camBuff = AssetMgr::GetRuntimeAsset<UniformBuffer>(camFilter);

            if(!camBuff)
                return;

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
               
                for (auto it : meshBuff->Uniforms)
                { 
                    shader->setUniformData(it.second); 
                }
                material->bind(shader);
                context->drawTriangles(meshBuff);
            }

            shader->unbind();

            context->onEndFrameBuffer(fbo);                      
        });
	}

    template<>
    bool RenderPass::is<GBufferPass>() const { return type() == RenderPassType::GBuffer; }
}