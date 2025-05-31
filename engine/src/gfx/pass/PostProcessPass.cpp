#include "PostProcessPass.h"
#include "UIPass.h"

#include "GlobalSettings.h"

#include "gfx/rhi/Shader.h"
#include "gfx/rhi/Texture.h"
#include "gfx/rhi/FrameBuffer.h"
#include "gfx/rhi/UniformBuffer.h"
#include "gfx/renderer/RenderContext.h"

#include "scene/node/CameraNode.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "PostProcessPass"

namespace pio
{
	void PostProcessPass::OnAttach(const Ref<RenderContext>& context)
	{
		m_Attrs.SetClear(Clear::Common())
			.SetCull(CullFace::Common())
			.SetBlend(Blend::Common())
			.SetDepth(DepthTest::Disable())
			.SetStencil(StencilTest::Disable());

		m_UIPass = CreateRef<UIPass>("UI", AfterRenderingPostProcessing);
		m_UIPass->OnAttach(context);
	}

	void PostProcessPass::OnExecute(const Ref<RenderContext>& context, const Ref<CameraNode>& camera)
	{
		auto colorBuff = context->GetRenderingData().Pipeline.DefferedBuffer->ColorBuffers()[0];
		context->SubmitRC([weakSelf = WeakRef<PostProcessPass>(Self<PostProcessPass>()), 
						   weakCtx = WeakRef<RenderContext>(context),
		                   weakCam = WeakRef<CameraNode>(camera),
		                   weakColorBuff = WeakRef<Texture>(colorBuff)]() mutable
		{
			auto self = weakSelf.lock();
			PIO_CHECK_RETURN(self, "self is invalid in PostProcessPass");
			auto ctx = weakCtx.lock();
			PIO_CHECK_RETURN(self, "context is invalid in PostProcessPass");
			auto cam = weakCam.lock();
			PIO_CHECK_RETURN(cam, "camera is invalid in PostProcessPass");
			auto* colorBuff = !weakColorBuff.expired() ? weakColorBuff.lock()->As<Texture2D>() : nullptr;
			PIO_CHECK_RETURN(colorBuff, "ColorBuffer is invalid in PostProcessPass");
			auto shader = ctx->FindShader(SHADER_SPEC_POSTPROCESS);
			PIO_CHECK_RETURN(shader, "shader is invalid in PostProcessPass");

			ctx->OnBeginFrameBuffer(cam->GetTarget(), self->GetRenderState());

			shader->Bind();

			colorBuff->BindAt(shader, colorBuff->Name());

			ctx->DrawTriangles(ctx->GetScreenMeshBuff());

			colorBuff->UnBind();

			shader->UnBind();
		});

		m_UIPass->OnExecute(context, camera);
	}
}