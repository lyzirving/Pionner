#include "GBufferPass.h"

#include "GlobalSettings.h"

#include "gfx/rhi/Shader.h"
#include "gfx/rhi/FrameBuffer.h"

#include "gfx/renderer/RenderContext.h"

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

        TextureSpecificBuilder texBuilder;
        texBuilder.name("GeoBuffer").type(TextureType::TwoDimen)
            .format(TextureFormat::RGB_24).width(colorW).height(colorH);
        fboSpec.ColorSpec.push_back(texBuilder.build());

        TextureSpecificBuilder depthBuilder;
        depthBuilder.name("GeoBuffPass Depth Buffer").type(TextureType::RenderBuffer)
            .format(TextureFormat::DEPTH_24).width(depthW).height(depthH);
        fboSpec.DepthSpec = depthBuilder.build();

        m_frameBuff = FrameBuffer::Create(context, fboSpec);
        context->uploadData(m_frameBuff);

        m_attrs.setClear(Clear::Common()).setBlend(Blend::Disable())
               .setDepth(DepthTest::Common()).setCull(CullFace::Common())
               .setStencil(StencilTest::Disable());        
	}

	void GBufferPass::onDetach(Ref<RenderContext>& context)
	{
        m_frameBuff.reset();
	}

	void GBufferPass::onExecute(Ref<RenderContext>& context, Ref<RenderPass>& lastPass)
	{
        auto &renderingData = context->renderingData();
        std::vector<MeshCmd> opaqueMesh = std::move(renderingData.OpaqueMeshCmd);
        if (opaqueMesh.empty())
        {
            return;
        }
        Ref<FrameBuffer>& fbo = m_frameBuff;
        Ref<Shader> &shader = context->shader(ShaderType::GBuffer);
        const RenderStateAttrs& attr = m_attrs;
		Ref<UniformBuffer> camBuff = renderingData.UnimBuffSet.get(UBBinding_Camera);

		context->submitRC([&context, &fbo, &shader, &attr, camBuff, opaqueMesh]() mutable
        {
            //context->onBeginFrameBuffer(fbo, attr);

            //shader->bind();

            ////[TODO]memory leak of camBuff
            //context->bindUnimBlock(shader, camBuff , "");
            //camBuff->bind();

            //for(auto &mesh : opaqueMesh)
            //{
            //    for (auto it : mesh.Uniforms) 
            //    { 
            //        shader->setUniformData(it.second); 
            //    }

            //    shader->setMaterial(mesh.Material);

            //    context->drawTriangles(mesh.Data.Vao, mesh.Data.Ebo);
            //}

            //shader->unbind();

            //context->onEndFrameBuffer(fbo);
        });
	}
}