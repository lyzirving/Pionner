#include "RenderPipeline.h"

#include "asset/AssetMgr.h"

#include "scene/Components.h"
#include "scene/3d/Camera.h"
#include "scene/node/CameraNode.h"
#include "scene/node/MeshNode.h"

#include "gfx/renderer/RenderContext.h"
#include "gfx/renderer/Renderer.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "RenderPipeline"

namespace pio
{
	RenderPipeline::RenderPipeline()
	{
	}

	void RenderPipeline::onAttach(Ref<RenderContext>& context)
	{
		m_renderer = Renderer::Create(GlobalSettings::RenderConfig);
		m_renderer->onAttach(context);
	}

	void RenderPipeline::onDetach(Ref<RenderContext>& context)
	{
		m_renderer->onDetach(context);
	}

	void RenderPipeline::onRender(Ref<RenderContext>& context, std::vector<Ref<CameraNode>>& camNodes)
	{
		onBeginFrameRendering(context);		

		for (int32_t i = 0; i < camNodes.size(); i++)
		{					
			camNodes[i]->update();
			onBeginCameraRendering(context, camNodes[i]);
			onRenderSingleCamera(context, camNodes[i]);
			onEndCameraRendering(context, camNodes[i]);
		}

		onEndFrameRendering(context);
	}

	void RenderPipeline::onBeginFrameRendering(Ref<RenderContext>& context)
	{
	}

	void RenderPipeline::onEndFrameRendering(Ref<RenderContext>& context)
	{
	}

	void RenderPipeline::onBeginCameraRendering(Ref<RenderContext>& context, Ref<CameraNode>& camNode)
	{
		context->onBeginFrameRendering();
	}

	void RenderPipeline::onRenderSingleCamera(Ref<RenderContext>& context, Ref<CameraNode>& camNode)
	{
		onInitializeRenderingData(context, camNode);

		m_renderer->onSetUp();

		m_renderer->onExecute(context, camNode);
	}

	void RenderPipeline::onEndCameraRendering(Ref<RenderContext>& context, Ref<CameraNode>& camNode)
	{
		context->onEndFrameRendering();
	}

	void RenderPipeline::onInitializeRenderingData(Ref<RenderContext>& context, Ref<CameraNode>& camNode)
	{		
		RenderingData renderingData;//Data to be committed
		auto& renderingNodes = context->renderingNodes();		
		auto camera = camNode->camera();

		camera->culling(renderingNodes);
		auto uBuff = camNode->getRenderingData<CameraNode, Ref<UniformBuffer>>();
		renderingData.UnimBuffSet.insert({ uBuff->binding(), uBuff->assetHnd() });

		onSetUpLight(context, renderingNodes, renderingData);

		onSetUpObject(context, renderingNodes, renderingData);

		context->setRenderingData(std::move(renderingData));
	}

	void RenderPipeline::onSetUpLight(Ref<RenderContext>& context, RenderingNodes& renderingNodes, RenderingData &renderingData)
	{
		auto& mainLight = renderingNodes.MainLight;
		//Pipeline::ProcessDirectionalLightEnt(context, mainLight, renderingData);
	}

	void RenderPipeline::onSetUpObject(Ref<RenderContext>& context, RenderingNodes& renderingNodes, RenderingData &renderingData)
	{
		auto &meshNodes = renderingNodes.Mesh;
		for (auto &mesh : meshNodes)
		{
			mesh->update();
			auto data = mesh->getRenderingData<MeshNode, MeshRenderingItem>();
			switch (data.Mode)
			{
				case RenderingMode_Opaque:
				{
					renderingData.OpaqueMeshItems.push_back(std::move(data));
					break;
				}
				case RenderingMode_Transparent:
				{
					renderingData.TransparentMeshItems.push_back(std::move(data));
					break;
				}
				default:
					break;
			}
		}
	}
}