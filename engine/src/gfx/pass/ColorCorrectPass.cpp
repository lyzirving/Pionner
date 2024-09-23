#include "ColorCorrectPass.h"
#include "DefferedPass.h"

#include "gfx/rhi/Shader.h"
#include "gfx/rhi/FrameBuffer.h"
#include "gfx/resource/RenderTarget.h"
#include "gfx/renderer/RenderContext.h"

#include "scene/node/CameraNode.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "ColorCorrectPass"

namespace pio
{
	void ColorCorrectPass::onAttach(Ref<RenderContext>& context)
	{
		m_attrs.setClear(Clear::Common())
			.setBlend(Blend::Disable())
			.setDepth(DepthTest::Common())
			.setCull(CullFace::Common())
			.setStencil(StencilTest::Disable());
	}

	void ColorCorrectPass::onExecute(Ref<RenderContext>& context, Ref<CameraNode>& camNode, Ref<RenderPass>& lastPass)
	{
		PIO_CHECK_RETURN(lastPass && lastPass->is<DefferedPass>(), "err! last pass must be DefferedPass");

		auto& lastFbo = lastPass->frameBuffer();

		auto& target = camNode->renderTarget();
		PIO_CHECK_RETURN(target, "err! render target has not been set!");

		auto& shader = context->shader(ShaderType::ColorCorrect);
		PIO_CHECK_RETURN(shader, "err! can't find ColorCorrect shader!");

		const auto& attr = m_attrs;

		context->submitRC([&context, &target, &lastFbo, &shader, &attr]()
		{
			const auto& colorBuffer = lastFbo->colorBuffers()[0];
			PIO_CHECK_RETURN(colorBuffer->is<Texture2D>(), "err! color buffer is not Texture2D");

			TextureSampler slot;
			auto* t = colorBuffer->as<Texture2D>();

			context->onBeginRenderTarget(target, attr);

			shader->bind();

			if (shader->getSampler(slot))
			{
				t->active(slot);
				t->bind();
				shader->setTextureSampler(GpuAttr::UNI_COLOR_BUFFER, slot);
			}
			else
			{
				LOGE("err! shader[%s] doesn't have enought texture sampler", shader->name().c_str());
			}

			context->drawTriangles(context->getScreenMeshBuffer());

			shader->unbind();
		});
	}

	template<>
	bool RenderPass::is<ColorCorrectPass>() const { return type() == RenderPassType::ColorCorrect; }
}