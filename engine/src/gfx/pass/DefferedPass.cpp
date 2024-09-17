#include "DefferedPass.h"
#include "GBufferPass.h"

#include "GlobalSettings.h"

#include "asset/AssetMgr.h"

#include "gfx/renderer/RenderContext.h"
#include "gfx/rhi/Shader.h"
#include "gfx/rhi/FrameBuffer.h"
#include "gfx/rhi/UniformBuffer.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "DefferedPass"

namespace pio
{
    void DefferedPass::onAttach(Ref<RenderContext>& context)
    {
        m_attrs.setClear(Clear::Common())
            .setBlend(Blend::Disable())
            .setDepth(DepthTest::Common())
            .setCull(CullFace::Common())
            .setStencil(StencilTest::Disable());

        auto colorSize = GlobalSettings::ColorResolution();
        auto depthSize = GlobalSettings::ShadowResolution();

        FrameBufferSpecific fboSpec;
        fboSpec.Name = "DefferedPass";
        fboSpec.Width = colorSize.x;
        fboSpec.Height = colorSize.y;
        PIO_FBO_ADD_USAGE(fboSpec.Usage, FrameBufferUsage_Color);

		TextureSpecificBuilder colorBufferSpec;
		colorBufferSpec.name(GpuAttr::UNI_COLOR_BUFFER)
			.type(TextureType::TwoDimen)
			.format(TextureFormat::RGBA_HALF)
			.width(colorSize.x).height(colorSize.y)
			.texWrap(TextureWrap::ClampEdge, TextureWrap::ClampEdge)
			.texFilter(TextureFilterMin::Linear, TextureFilterMag::Linear);

        TextureSpecificBuilder depthBuilder;
        depthBuilder.name("DefferedPass Depth Buffer")
            .type(TextureType::RenderBuffer)
            .format(TextureFormat::DEPTH_24_STENCIL_8)
            .width(depthSize.x).height(depthSize.y);

        fboSpec.ColorSpec.push_back(colorBufferSpec.build());
        fboSpec.DepthSpec = depthBuilder.build();

        m_frameBuff = FrameBuffer::Create(context, fboSpec);
        context->uploadData(m_frameBuff);
    }

    void DefferedPass::onDetach(Ref<RenderContext>& context)
    {
        m_frameBuff.reset();
    }

	void DefferedPass::onExecute(Ref<RenderContext>& context, Ref<CameraNode>& camNode, Ref<RenderPass>& lastPass)
    {
        auto& renderingData = context->renderingData();
        PIO_CHECK_RETURN(lastPass && lastPass->is<GBufferPass>(), "err! last pass must be GBufferPass");

        auto& lastFbo = lastPass->frameBuffer();
        auto& fbo = m_frameBuff;
        auto& shader = context->shader(ShaderType::Deferred);
        const auto& attr = m_attrs;

        auto it = renderingData.UnimBuffSet.find(UBBinding_Camera);
        PIO_CHECK_RETURN(it != renderingData.UnimBuffSet.end(), "err! fail to find camera buff id");
        UUID32 camFilter = it->second;

        it = renderingData.UnimBuffSet.find(UBBinding_DirectionalLight);
        PIO_CHECK_RETURN(it != renderingData.UnimBuffSet.end(), "err! fail to find directional light buff id");
        UUID32 lightFilter = it->second;

        context->submitRC([camFilter, lightFilter, &context, &fbo, &lastFbo, &shader, &attr]()
        {
            auto camBuff = AssetMgr::GetRuntimeAsset<UniformBuffer>(camFilter);
            PIO_CHECK_RETURN(camBuff, "err! invalid camera uniform buffer");
            auto lightBuff = AssetMgr::GetRuntimeAsset<UniformBuffer>(lightFilter);
            PIO_CHECK_RETURN(lightBuff, "err! invalid directional light uniform buffer");

            const auto& colorBuffers = lastFbo->colorBuffers();
           
            context->onBeginFrameBuffer(fbo, attr);

            shader->bind();

            context->bindUnimBlock(shader, camBuff, GpuAttr::BINDING_CAM_BLOCK);
            camBuff->bind();

            context->bindUnimBlock(shader, lightBuff, GpuAttr::BINDING_DIRECTIONAL_LIGHT_BLOCK);
            lightBuff->bind();

            TextureSampler slot;
            for (size_t i = 0; i < colorBuffers.size(); i++)
            {
                const auto& t = colorBuffers[i];
                if (t->is<Texture2D>())
                {
                    auto* p = t->as<Texture2D>();
                    if (shader->getSampler(slot))
                    {
                        p->active(slot);
                        p->bind();
                        shader->setTextureSampler(p->name(), slot);
                    }
                    else
                    {
                        LOGE("err! fail to get available sampler for texture[%s]", p->name().c_str());
                        break;
                    }
                }
            }

            context->drawTriangles(context->getScreenMeshBuffer());

            lightBuff->unbind();

            camBuff->unbind();

            shader->unbind();

            context->onEndFrameBuffer(fbo);
        });
    }

    template<>
    bool RenderPass::is<DefferedPass>() const { return type() == RenderPassType::Deferred; }
}