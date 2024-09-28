#include "MainLightShadowCasterPass.h"

#include "GlobalSettings.h"

#include "asset/AssetMgr.h"

#include "gfx/renderer/RenderContext.h"
#include "gfx/rhi/Shader.h"
#include "gfx/rhi/FrameBuffer.h"
#include "gfx/rhi/UniformBuffer.h"

#include "gfx/resource/Mesh.h"
#include "gfx/resource/MeshRenderBuffer.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "MainLightShadowCasterPass"

namespace pio
{
	void MainLightShadowCasterPass::onAttach(Ref<RenderContext>& context)
	{
		auto shadowSize = GlobalSettings::ShadowResolution().x;

		FrameBufferSpecific fboSpec;
		fboSpec.Name = "MainLightShadowCasterPass";
		fboSpec.Width = shadowSize;
		fboSpec.Height = shadowSize;
		PIO_FBO_ADD_USAGE(fboSpec.Usage, FrameBufferUsage_Depth);

		TextureSpecificBuilder depthBuilder;
		depthBuilder.name("MainLightDepthBuffer")
			.type(TextureType::TwoDimen)
			.format(TextureFormat::DEPTH_32F)
			.width(shadowSize).height(shadowSize)
			.texWrap(TextureWrap::ClampBorder, TextureWrap::ClampBorder)
			.border(glm::vec4(1.f))
			.texFilter(TextureFilterMin::Nearest, TextureFilterMag::Nearest);

		fboSpec.DepthSpec = depthBuilder.build();
		m_frameBuff = FrameBuffer::Create(context, fboSpec);
		context->uploadData(m_frameBuff);

		m_attrs.setClear(Clear::Common())
			.setBlend(Blend::Disable())
			.setDepth(DepthTest::Common())				
			.setCull(CullFace::Common())
			.setStencil(StencilTest::Disable());
	}

	void MainLightShadowCasterPass::onDetach(Ref<RenderContext>& context)
	{
		m_frameBuff.reset();
	}

	void MainLightShadowCasterPass::onExecute(Ref<RenderContext>& context, Ref<CameraNode>& camNode, Ref<RenderPass>& lastPass)
	{
		auto& renderingData = context->renderingData();
		std::vector<MeshRenderingItem> opaqueMeshItems = renderingData.OpaqueMeshItems;
		if (opaqueMeshItems.empty())  { return; }

		auto it = renderingData.UnimBuffSet.find(UBBinding_DirectionalLightShadow);
		PIO_CHECK_RETURN(it != renderingData.UnimBuffSet.end(), "err! fail to find directional light shadow data");
		UUID32 shadowFilter = it->second;

		auto& fbo = m_frameBuff;
		auto& shader = context->shader(ShaderType::MainLightShadowCaster);
		const auto& attr = m_attrs;

		context->submitRC([shadowFilter, opaqueMeshItems, &context, &fbo, &shader, &attr]()
		{
			auto shadowData = AssetMgr::GetRuntimeAsset<UniformBuffer>(shadowFilter);
			PIO_CHECK_RETURN(shadowData, "err! invalid shadow data uniform data");

			context->onBeginFrameBuffer(fbo, attr);

			shader->bind();

			context->bindUnimBlock(shader, shadowData, GpuAttr::BINDING_MAIN_LIGHT_CASTER_BLOCK);
			shadowData->bind();

			for(const auto& item : opaqueMeshItems)
			{
				Ref<MeshRenderBuffer> meshBuff = AssetMgr::GetRuntimeAsset<MeshRenderBuffer>(item.RenderBuffFilter);
				if(!meshBuff)
				{
					LOGW("warning! fail to find mesh render buffer or material in asset, it might be deleted");
					continue;
				}
				meshBuff->bind(shader);
				context->drawTriangles(meshBuff);
			}

			shadowData->unbind();

			shader->unbind();
		});
	}

	template<>
	bool RenderPass::is<MainLightShadowCasterPass>() const { return type() == RenderPassType::MainLightShadowCaster; }
}