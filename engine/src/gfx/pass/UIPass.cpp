#include "UIPass.h"

#include "asset/material/Material.h"

#include "gfx/rhi/Shader.h"
#include "gfx/rhi/Texture.h"
#include "gfx/rhi/FrameBuffer.h"
#include "gfx/rhi/UniformBuffer.h"
#include "gfx/renderer/RenderContext.h"

#include "scene/node/CameraNode.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "UIPass"

namespace pio
{
	void UIPass::OnAttach(const Ref<RenderContext>& context)
	{
		m_Attrs.SetClear(Clear::Disable())
			.SetCull(CullFace::Common())
			.SetBlend(Blend::Common())
			.SetDepth(DepthTest::Disable())
			.SetStencil(StencilTest::Disable());
	}

	void UIPass::OnExecute(const Ref<RenderContext>& context, const Ref<CameraNode>& camera)
	{
		context->SubmitRC([weakSelf = WeakRef<UIPass>(Self<UIPass>()),
						  weakCtx = WeakRef<RenderContext>(context),
						  weakCam = WeakRef<CameraNode>(camera),
						  data = context->GetRenderingData()]() mutable
		{
			auto self = weakSelf.lock();
			PIO_CHECK_RETURN(self, "self is invalid in UIPass");
			auto ctx = weakCtx.lock();
			PIO_CHECK_RETURN(ctx, "context is invalid in UIPass");
			auto cam = weakCam.lock();
			PIO_CHECK_RETURN(cam, "camera is invalid in UIPass");
			auto camUBuff = data.UBuffs[UBBinding_Camera];
			PIO_CHECK_RETURN(camUBuff, "camera is invalid in UIPass");

			ctx->OnBeginFrameBuffer(cam->GetTarget(), self->GetRenderState());
			for(size_t i = 0; i < data.WireframeItems.size(); i++)
			{
				auto& item = data.WireframeItems[i];
				auto shader = item.Material->GetShader();
				if(!shader)
					continue;

				shader->Bind();

				camUBuff->BindBlock(ctx, shader);

				item.Material->Bind(shader);
				for(auto& buff : item.UniformBuffers)
					buff->BindBlock(ctx, shader);

				ctx->DrawLines(item.MeshBuff);

				item.Material->UnBind();
				camUBuff->UnBind();
				shader->UnBind();
			}

			for(size_t i = 0; i < data.SpriteItems.size(); i++)
			{
				auto& item = data.SpriteItems[i];
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
				camUBuff->UnBind();
				shader->UnBind();
			}
		});
	}
}