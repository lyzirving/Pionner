#include "UiPass.h"

#include "asset/AssetMgr.h"

#include "gfx/rhi/Shader.h"
#include "gfx/rhi/UniformBuffer.h"

#include "gfx/resource/RenderTarget.h"
#include "gfx/renderer/RenderContext.h"
#include "gfx/resource/material/Material.h"
#include "gfx/resource/MeshRenderBuffer.h"

#include "scene/node/CameraNode.h"

namespace pio
{
	void UiPass::onAttach(Ref<RenderContext>& context)
	{
		m_attrs.setClear(Clear::Common())
			.setBlend(Blend::Common())
			.setDepth(DepthTest::Disable())
			.setCull(CullFace::Disable())
			.setStencil(StencilTest::Disable());
	}

	void UiPass::onExecute(Ref<RenderContext>& context, Ref<CameraNode>& camNode, Ref<RenderPass>& lastPass)
	{
		auto& renderingData = context->renderingData();
		auto uiSprites = renderingData.UiSprites;
		auto outlines = renderingData.Outlines;
		if (uiSprites.empty() || outlines.empty()) { return; }

		auto it = renderingData.UnimBuffSet.find(UBBinding_Camera);
		PIO_CHECK_RETURN(it != renderingData.UnimBuffSet.end(), "err! fail to find camera buff id");
		UUID32 camFilter = it->second;

		auto& target = camNode->renderTarget();
		PIO_CHECK_RETURN(target, "err! render target has not been set!");

		const auto& attr = m_attrs;

		context->submitRC([&context, &target, &attr, camFilter, uiSprites, outlines]()
		{
			auto camBuff = AssetMgr::GetRuntimeAsset<UniformBuffer>(camFilter);
			PIO_CHECK_RETURN(camBuff, "err! invalid camera uniform buffer");			

			context->onBeginRenderTarget(target, attr);	

			if (!uiSprites.empty())
			{
				auto spriteShader = context->getShader(ShaderType::Sprite);				
				spriteShader->bind();

				context->bindUnimBlock(spriteShader, camBuff, GpuAttr::BINDING_CAM_BLOCK);
				camBuff->bind();

				for (auto& item : uiSprites)
				{
					if (item.Mode != RenderingMode_Overlay)
					{
						LOGW("warning! ui sprite's rendering mode is not overlay");
						continue;
					}
					Ref<MeshRenderBuffer> meshBuff = AssetMgr::GetRuntimeAsset<MeshRenderBuffer>(item.RenderBuffFilter);
					Ref<Material> material = AssetMgr::GetRuntimeAsset<Material>(item.MaterialFilter);
					if (!meshBuff || !material)
					{
						LOGW("warning! fail to find mesh render buffer or material in asset, it might be deleted");
						continue;
					}

					meshBuff->bind(spriteShader);
					material->bind(spriteShader);
					context->drawTriangles(meshBuff);
					spriteShader->freeSamplerUnit();
				}
				camBuff->unbind();
				spriteShader->unbind();
			}

			if (!outlines.empty())
			{
				auto outlineShader = context->getShader(ShaderType::Outline);
				outlineShader->bind();

				context->bindUnimBlock(outlineShader, camBuff, GpuAttr::BINDING_CAM_BLOCK);
				camBuff->bind();

				for (auto& item : outlines)
				{
					if (item.Mode != RenderingMode_Overlay)
					{
						LOGW("warning! ui mesh's rendering mode is not overlay");
						continue;
					}
					Ref<MeshRenderBuffer> meshBuff = AssetMgr::GetRuntimeAsset<MeshRenderBuffer>(item.RenderBuffFilter);
					Ref<Material> material = AssetMgr::GetRuntimeAsset<Material>(item.MaterialFilter);
					if (!meshBuff || !material)
					{
						LOGW("warning! fail to find mesh render buffer or material in asset, it might be deleted");
						continue;
					}
					meshBuff->bind(outlineShader);
					material->bind(outlineShader);
					context->drawLines(meshBuff);
					outlineShader->freeSamplerUnit();
				}
				camBuff->unbind();
				outlineShader->unbind();
			}
		});
	}

	template<>
	bool RenderPass::is<UiPass>() const { return type() == RenderPassType::UI; }
}
