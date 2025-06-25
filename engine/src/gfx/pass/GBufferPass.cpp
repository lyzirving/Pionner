#include "GBufferPass.h"

#include "GlobalSettings.h"

#include "asset/material/Material.h"

#include "gfx/rhi/Shader.h"
#include "gfx/rhi/FrameBuffer.h"
#include "gfx/rhi/UniformBuffer.h"
#include "gfx/rhi/UniformData.h"

#include "gfx/renderer/GBuffer.h"
#include "gfx/renderer/MeshRenderBuffer.h"
#include "gfx/renderer/RenderContext.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "GBufferPass"

namespace pio
{
	void GBufferPass::OnAttach(const Ref<RenderContext>& context)
	{
		m_FrameBuff = CreateRef<GBuffer>(context, "GBuffer");

		StencilTest stencil = StencilTest::Common();
		stencil.SetMask(0xff);
		stencil.SetOp(StencilOp(FuncAttr::Keep, FuncAttr::Keep, FuncAttr::Replace));
		stencil.SetFunc(StencilFunc(FuncAttr::Always, GpuAttr::Stencil::VALID_GEOMETRY, 0xff));
		//no transparent shading
		m_Attrs.SetClear(Clear::Common())
			.SetBlend(Blend::Disable())
			.SetDepth(DepthTest::Common())
            .SetCull(CullFace::Common())
			.SetStencil(stencil);
	}

	void GBufferPass::OnDetach(const Ref<RenderContext>& context)
	{
        m_FrameBuff.reset();
	}

	void GBufferPass::OnExecute(const Ref<RenderContext>& context, const Ref<CameraNode>& camera)
	{
		context->GetRenderingData().Pipeline.GBuffer = m_FrameBuff;
		context->SubmitRC([weakSelf = WeakRef<GBufferPass>(Self<GBufferPass>()), weakCtx = WeakRef<RenderContext>(context),
						  //RenderingData must be copied for cross thread work
		                  data = context->GetRenderingData()]() mutable
		{
			auto self = weakSelf.lock();
			PIO_CHECK_RETURN(self, "self is invalid in GBufferPass");
			auto ctx = weakCtx.lock();
			PIO_CHECK_RETURN(ctx, "context is invalid in GBufferPass");
			auto camUBuff = data.UBuffs[UBBinding_Camera];
			PIO_CHECK_RETURN(camUBuff, "camera is invalid in GBufferPass");

			ctx->OnBeginFrameBuffer(self->GetFrameBuffer(), self->GetRenderState());
			for(size_t i = 0; i < data.OpaqueMeshItems.size(); i++)
			{
				auto& item = data.OpaqueMeshItems[i];
				auto shader = item.Material->GetShader();
				if(!shader)
					continue;

				shader->Bind();

				camUBuff->BindBlock(ctx, shader);

				item.Material->Bind(shader);

				for(auto& buff : item.UniformBuffers)
					buff->BindBlock(ctx, shader);

				ctx->DrawTriangles(item.MeshBuff);				

				item.Material->UnBind();

				for(auto& buff : item.UniformBuffers)
					buff->UnBind();

				camUBuff->UnBind();
				shader->UnBind();
			}
		});
	}
}