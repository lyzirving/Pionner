#include "DefferedPass.h"
#include "GBufferPass.h"

#include "GlobalSettings.h"

#include "gfx/renderer/RenderContext.h"
#include "gfx/rhi/FrameBuffer.h"
#include "gfx/rhi/Shader.h"
#include "gfx/resource/MeshRenderBuffer.h"

#include "scene/Factory.h"
#include "scene/resources/Mesh.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "DefferedPass"

namespace pio
{
    void DefferedPass::onAttach(Ref<RenderContext>& context)
    {
        uint32_t colorW = GlobalSettings::ColorResolution(GlobalSettings::ColorResoSetting);      
        uint32_t colorH = colorW / GlobalSettings::AspectRatio(GlobalSettings::AspectSetting);

        uint32_t depthW = GlobalSettings::ShadowResolution(GlobalSettings::ShadowResoSetting);
        uint32_t depthH = depthW / GlobalSettings::AspectRatio(GlobalSettings::AspectSetting);

        FrameBufferSpecific fboSpec;
        fboSpec.Name = "DefferedPass";
        fboSpec.Width = colorW;
        fboSpec.Height = colorH;
        PIO_FBO_ADD_USAGE(fboSpec.Usage, FrameBufferUsage_Color);

        TextureSpecificBuilder texBuilder;
        texBuilder.name("Deferred Pass Color Buffer")
            .type(TextureType::TwoDimen)
            .format(TextureFormat::RGBA_HALF)
            .width(colorW).height(colorH)
            .texWrap(TextureWrap::ClampEdge, TextureWrap::ClampEdge)
            .texFilter(TextureFilterMin::Nearest, TextureFilterMag::Nearest);       

        TextureSpecificBuilder depthBuilder;
        depthBuilder.name("Deferred Pass Depth Buffer")
            .type(TextureType::RenderBuffer)
            .format(TextureFormat::DEPTH_24_STENCIL_8)
            .width(depthW).height(depthH);

        fboSpec.ColorSpec.push_back(texBuilder.build()); 
        fboSpec.DepthSpec = depthBuilder.build();

        m_frameBuff = FrameBuffer::Create(context, fboSpec);
        context->uploadData(m_frameBuff);

        m_attrs.setClear(Clear::Common())
            .setBlend(Blend::Disable())
            .setDepth(DepthTest::Common())
            .setCull(CullFace::Common())
            .setStencil(StencilTest::Disable());

        m_screenMesh = CreateRef<Mesh>();
        m_screenBuff = CreateRef<MeshRenderBuffer>();

        m_screenMesh->setTriangleMesh(GeometryFactory::MakeScreenQuad());
        m_screenBuff->update(context, m_screenMesh);
    }

    void DefferedPass::onDetach(Ref<RenderContext>& context)
    {
        m_screenMesh.reset();
        m_screenBuff.reset();
        m_frameBuff.reset();
    }

	void DefferedPass::onExecute(Ref<RenderContext>& context, Ref<RenderPass>& lastPass)
    {
        if (!lastPass || !lastPass->is<GBufferPass>())
        {
            LOGE("err! invalid last pass");
            return;
        }
        auto& lastFbo = lastPass->frameBuffer();
        auto& fbo = m_frameBuff;
        auto& shader = context->shader(ShaderType::Deferred);
        auto& meshBuff = m_screenBuff;
        const auto& attr = m_attrs;

        context->submitRC([&context, &fbo, &lastFbo, &shader, &meshBuff, &attr]()
        {
            const auto& colorBuffers = lastFbo->colorBuffers();

            context->onBeginFrameBuffer(fbo, attr);

            shader->bind();

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

            context->drawTriangles(meshBuff);

            shader->unbind();

            context->onEndFrameBuffer(fbo);
        });
    }

    template<>
    bool RenderPass::is<DefferedPass>() const { return type() == RenderPassType::Deferred; }
}