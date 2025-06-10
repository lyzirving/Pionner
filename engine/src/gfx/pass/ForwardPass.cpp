#include "ForwardPass.h"

#include "GlobalSettings.h"

#include "gfx/renderer/RenderContext.h"
#include "gfx/renderer/ShadowMap.h"
#include "gfx/renderer/PointLitShadowMap.h"

#include "gfx/rhi/Shader.h"
#include "gfx/rhi/FrameBuffer.h"
#include "gfx/rhi/UniformBuffer.h"
#include "gfx/rhi/Texture.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "ForwardPass"

namespace pio
{
    void ForwardPass::OnAttach(const Ref<RenderContext>& context)
    {
        m_Attrs.SetClear(Clear::Common())
            .SetCull(CullFace::Common())
            .SetBlend(Blend::Common())
            .SetDepth(DepthTest::Common())            
            .SetStencil(StencilTest::Disable());
    }

    void ForwardPass::OnDetach(const Ref<RenderContext>& context)
    {       
    }

	void ForwardPass::OnExecute(const Ref<RenderContext>& context, const Ref<CameraNode>& camera)
    {        
    }
}