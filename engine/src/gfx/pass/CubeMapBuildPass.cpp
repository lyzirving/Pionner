#include "CubeMapBuildPass.h"

#include "GlobalSettings.h"

#include "gfx/rhi/Shader.h"
#include "gfx/rhi/FrameBuffer.h"
#include "gfx/rhi/UniformBuffer.h"
#include "gfx/rhi/Texture.h"
#include "gfx/renderer/RenderContext.h"

#include "asset/image/ImageHDR.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "CubeMapBuildPass"

namespace pio
{
	CubeMapBuildPass::CubeMapBuildPass(const Ref<ImageHDR>& image, const std::string& name)
		: RenderPass(name, BeforeRendering),//Pass as a tool should be executed at the every first of rendering
		m_ImageHDR(image)
	{
	}

	void CubeMapBuildPass::OnAttach(const Ref<RenderContext>& context)
	{
        m_Attrs.SetClear(Clear::Common())
            .SetCull(CullFace::Common())
            .SetBlend(Blend::Disable())
            .SetDepth(DepthTest::Disable())
            .SetStencil(StencilTest::Disable());

        auto imgHDR = m_ImageHDR.lock();
        int32_t size = std::min(imgHDR->GetWidth(), imgHDR->GetHeight());

        FrameBufferSpecific fboSpec;
        fboSpec.Name = Name();
        fboSpec.Width = size;
        fboSpec.Height = size;
        PIO_FBO_ADD_USAGE(fboSpec.Usage, FrameBufferUsage_Color);

        TextureSpecificBuilder cubeMapSpec;
        cubeMapSpec.Name(std::string(imgHDR->Name()).append("-CubeMap"))
            .Type(TextureType::CubeMap)
            .Format(TextureFormat::RGB_16F)
            .Width(size).Height(size)
            .TexWrap(TextureWrap::ClampEdge, TextureWrap::ClampEdge)
            .TexFilter(TextureFilterMin::Linear, TextureFilterMag::Linear);

        fboSpec.ColorSpec.push_back(cubeMapSpec.Build());

        m_FrameBuff = FrameBuffer::Create(context, fboSpec);

        context->UploadData(m_FrameBuff);
	}

	void CubeMapBuildPass::OnDetach(const Ref<RenderContext>& context)
	{
	}

	void CubeMapBuildPass::OnExecute(const Ref<RenderContext>& context, const Ref<CameraNode>& camera)
	{
	}
}