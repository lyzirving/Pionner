#include "DefferedPass.h"
#include "GBufferPass.h"

#include "GlobalSettings.h"

#include "gfx/renderer/RenderContext.h"
#include "gfx/rhi/FrameBuffer.h"
#include "gfx/rhi/Shader.h"
#include "gfx/resource/MeshRenderBuffer.h"
#include "gfx/resource/RenderTarget.h"
#include "gfx/resource/Mesh.h"

#include "scene/Factory.h"
#include "scene/node/CameraNode.h"

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

        m_screenMesh = CreateRef<Mesh>();
        m_screenBuff = CreateRef<MeshRenderBuffer>();

        m_screenMesh->setTriangleMesh(Factory::MakeScreenQuad());
        m_screenBuff->update(context, m_screenMesh);
    }

    void DefferedPass::onDetach(Ref<RenderContext>& context)
    {
        m_screenMesh.reset();
        m_screenBuff.reset();
    }

	void DefferedPass::onExecute(Ref<RenderContext>& context, Ref<CameraNode>& camNode, Ref<RenderPass>& lastPass)
    {
        PIO_CHECK_RETURN(lastPass && lastPass->is<GBufferPass>(), "err! invalid last pass");
        auto& target = camNode->renderTarget();
        PIO_CHECK_RETURN(target, "err! render target has not been set!");

        auto& lastFbo = lastPass->frameBuffer();
        auto& shader = context->shader(ShaderType::Deferred);
        auto& meshBuff = m_screenBuff;
        const auto& attr = m_attrs;

        context->submitRC([&context, &target, &lastFbo, &shader, &meshBuff, &attr]()
        {
            const auto& colorBuffers = lastFbo->colorBuffers();

            context->onBeginRenderTarget(target, attr);

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

            context->onEndRenderTarget(target);
        });
    }

    template<>
    bool RenderPass::is<DefferedPass>() const { return type() == RenderPassType::Deferred; }
}