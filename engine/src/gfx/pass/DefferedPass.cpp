#include "DefferedPass.h"

#include "GlobalSettings.h"

#include "gfx/renderer/RenderContext.h"
#include "gfx/rhi/FrameBuffer.h"

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
        texBuilder.name("DefferedPass Color Buffer").type(TextureType::TwoDimen)
            .format(TextureFormat::RGB_24).width(colorW).height(colorH);
        fboSpec.ColorSpec.push_back(texBuilder.build());

        TextureSpecificBuilder depthBuilder;
        depthBuilder.name("DefferedPass Depth Buffer").type(TextureType::RenderBuffer)
            .format(TextureFormat::DEPTH_24).width(depthW).height(depthH);
        fboSpec.DepthSpec = depthBuilder.build();

        m_frameBuff = FrameBuffer::Create(context, fboSpec);
        context->uploadData(m_frameBuff);
    }

    void DefferedPass::onDetach(Ref<RenderContext>& context)
    {
        m_frameBuff.reset();
    }

	void DefferedPass::onExecute(Ref<RenderContext>& context, Ref<RenderPass>& lastPass)
    {
    }
}