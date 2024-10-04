#include "MainLightShadowCasterPass.h"

#include "GlobalSettings.h"

#include "asset/AssetMgr.h"

#include "gfx/renderer/RenderContext.h"
#include "gfx/rhi/Shader.h"
#include "gfx/rhi/FrameBuffer.h"
#include "gfx/rhi/UniformBuffer.h"

#include "gfx/resource/Mesh.h"
#include "gfx/resource/MeshRenderBuffer.h"

#include "scene/3d/ShadowMap.h"
#include "scene/3d/CascadeShadowMap.h"
#include "scene/3d/ShadowMapFrameBuffer.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "MainLightShadowCasterPass"

namespace pio
{
	void MainLightShadowCasterPass::onAttach(Ref<RenderContext>& context)
	{
		m_attrs.setClear(Clear::Common())
			.setBlend(Blend::Disable())
			.setDepth(DepthTest::Common())				
			.setCull(CullFace::Common())
			.setStencil(StencilTest::Disable());
	}

	void MainLightShadowCasterPass::onExecute(Ref<RenderContext>& context, Ref<CameraNode>& camNode, Ref<RenderPass>& lastPass)
	{
		auto& renderingData = context->renderingData();
		std::vector<MeshRenderingItem> opaqueMeshItems = renderingData.OpaqueMeshItems;
		if (opaqueMeshItems.empty())  { return; }
		
		const auto& attr = m_attrs;

		context->submitRC([opaqueMeshItems, &context, &attr]()
		{
			auto& shader = context->shader(ShaderType::MainLightShadowCaster);
			//TODO
			/*{
				auto* csm = context->getLightTech(LightTech::CascadeShadowMap)->as<CascadeShadowMap>();
				auto& fbo = csm->frameBuff();
				auto& uBuff = csm->UBuff();
				for (size_t i = 0; i < CASCADE_NUM; i++)
				{
					context->onBeginFrameBuffer(fbo, attr, i);
					shader->bind();
					shader->setInt("u_lightTech", (int32_t)LightTech::CascadeShadowMap);
					shader->setInt("u_cascadeIdx", i);

					context->bindUnimBlock(shader, uBuff, GpuAttr::BINDING_CASCADE_SHADOW_MAP);
					uBuff->bind();

					for (const auto& item : opaqueMeshItems)
					{
						Ref<MeshRenderBuffer> meshBuff = AssetMgr::GetRuntimeAsset<MeshRenderBuffer>(item.RenderBuffFilter);
						if (!meshBuff)
						{
							LOGW("warning! fail to find mesh render buffer or material in asset, it might be deleted");
							continue;
						}
						meshBuff->bind(shader);
						context->drawTriangles(meshBuff);
					}

					uBuff->unbind();
					shader->unbind();
					context->onEndFrameBuffer(fbo);
				}
			}*/

			auto* sdMap = context->getLightTech(LightTech::ShadowMap)->as<ShadowMap>();
			auto& sdMapFbo = sdMap->frameBuff();
			auto& sdMapBuff = sdMap->UBuff();

			context->onBeginFrameBuffer(sdMapFbo, attr);
			shader->bind();
			shader->setInt("u_lightTech", (int32_t)LightTech::ShadowMap);

			context->bindUnimBlock(shader, sdMapBuff, GpuAttr::BINDING_MAIN_LIGHT_CASTER_BLOCK);
			sdMapBuff->bind();

			for (const auto& item : opaqueMeshItems)
			{
				Ref<MeshRenderBuffer> meshBuff = AssetMgr::GetRuntimeAsset<MeshRenderBuffer>(item.RenderBuffFilter);
				if (!meshBuff)
				{
					LOGW("warning! fail to find mesh render buffer or material in asset, it might be deleted");
					continue;
				}
				meshBuff->bind(shader);
				context->drawTriangles(meshBuff);
			}

			sdMapBuff->unbind();
			shader->unbind();		
		});
	}

	template<>
	bool RenderPass::is<MainLightShadowCasterPass>() const { return type() == RenderPassType::MainLightShadowCaster; }
}