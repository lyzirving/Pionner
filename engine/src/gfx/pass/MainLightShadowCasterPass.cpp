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
	void MainLightShadowCasterPass::onExecute(Ref<RenderContext>& context, Ref<CameraNode>& camNode, Ref<RenderPass>& lastPass)
	{
		auto& renderingData = context->renderingData();
		std::vector<ShadowCastItem> shadowCastItems = renderingData.ShadowCastItems;
		if (shadowCastItems.empty())  { return; }	

		context->submitRC([shadowCastItems, &context]()
		{
			auto& shader = context->getShader(ShaderType::MainLightShadowCaster);
			auto lightTechIdx = GlobalSettings::RenderConfig.LightingTech;
			auto lightTech = context->getLightTech(lightTechIdx);
			switch (lightTechIdx)
			{
				case LightTech::ShadowMap:
				{					
					auto& sdMapFbo = lightTech->frameBuff();					

					context->onBeginFrameBuffer(sdMapFbo, lightTech->renderState());
					shader->bind();
					shader->setInt("u_lightTech", (int32_t)LightTech::ShadowMap);

					lightTech->bindUnimBlock(context, shader);

					for (const auto& item : shadowCastItems)
					{
						Ref<MeshRenderBuffer> meshBuff = AssetMgr::GetRuntimeAsset<MeshRenderBuffer>(item.RenderBuffFilter);
						if (!meshBuff)
						{
							LOGW("warning! fail to find mesh render buffer or material in asset, it might be deleted");
							continue;
						}
						context->setCullFace(item.Mode == ShadowCastMode_TwoSided ? CullFace::Disable() : lightTech->renderState().AttrCull);
						meshBuff->bind(shader);
						context->drawTriangles(meshBuff);
						shader->freeSamplerUnit();
					}
					lightTech->unbindUnimBlock();
					shader->unbind();
					break;
				}
				case LightTech::CascadeShadowMap:
				{					
					auto& cmsFbo = lightTech->frameBuff();

					for (size_t i = 0; i < CASCADE_NUM; i++)
					{
						context->onBeginFrameBuffer(cmsFbo, lightTech->renderState(), i);
						shader->bind();
						shader->setInt("u_lightTech", (int32_t)LightTech::CascadeShadowMap);
						shader->setInt("u_cascadeIdx", i);

						lightTech->bindUnimBlock(context, shader);

						for (const auto& item : shadowCastItems)
						{
							Ref<MeshRenderBuffer> meshBuff = AssetMgr::GetRuntimeAsset<MeshRenderBuffer>(item.RenderBuffFilter);
							if (!meshBuff)
							{
								LOGW("warning! fail to find mesh render buffer or material in asset, it might be deleted");
								continue;
							}
							context->setCullFace(item.Mode == ShadowCastMode_TwoSided ? CullFace::Disable() : lightTech->renderState().AttrCull);
							meshBuff->bind(shader);
							context->drawTriangles(meshBuff);
							shader->freeSamplerUnit();
						}
						lightTech->unbindUnimBlock();
						shader->unbind();						
					}
					break;
				}
				default:
				{
					LOGE("err! unimplemened light tech[%u]", lightTechIdx);
					break;
				}
			}				
		});
	}

	template<>
	bool RenderPass::is<MainLightShadowCasterPass>() const { return type() == RenderPassType::MainLightShadowCaster; }
}