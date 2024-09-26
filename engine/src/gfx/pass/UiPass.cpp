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
		std::vector<MeshRenderingItem> uiSprites = renderingData.UiSprites;
		if (uiSprites.empty()) { return; }

		auto it = renderingData.UnimBuffSet.find(UBBinding_Camera);
		PIO_CHECK_RETURN(it != renderingData.UnimBuffSet.end(), "err! fail to find camera buff id");
		UUID32 camFilter = it->second;

		auto& target = camNode->renderTarget();
		PIO_CHECK_RETURN(target, "err! render target has not been set!");

		auto& shader = context->shader(ShaderType::Sprite);
		const auto& attr = m_attrs;

		context->submitRC([&context, &target, &shader, &attr, camFilter, uiSprites]()
		{
			auto camBuff = AssetMgr::GetRuntimeAsset<UniformBuffer>(camFilter);
			PIO_CHECK_RETURN(camBuff, "err! invalid camera uniform buffer");

			context->onBeginRenderTarget(target, attr);

			shader->bind();

			shader->setBool("u_bOverlay", true);

			context->bindUnimBlock(shader, camBuff, GpuAttr::BINDING_CAM_BLOCK);
			camBuff->bind();			

			for (auto& item : uiSprites)
			{
				if (item.Mode != RenderingMode_Overlay)
				{
					LOGW("warning! rendering mode is not overlay");
					continue;
				}
				Ref<MeshRenderBuffer> meshBuff = AssetMgr::GetRuntimeAsset<MeshRenderBuffer>(item.RenderBuffFilter);
				Ref<Material> material = AssetMgr::GetRuntimeAsset<Material>(item.MaterialFilter);
				if (!meshBuff || !material)
				{
					LOGW("warning! fail to find mesh render buffer or material in asset, it might be deleted");
					continue;
				}

				meshBuff->bind(shader);
				material->bind(shader);
				context->drawTriangles(meshBuff);
			}

			camBuff->unbind();

			shader->unbind();
		});
	}

	template<>
	bool RenderPass::is<UiPass>() const { return type() == RenderPassType::UI; }
}
