#include "RenderPipeline.h"

#include "asset/AssetMgr.h"

#include "scene/Components.h"
#include "scene/3d/Camera.h"

#include "scene/node/CameraNode.h"
#include "scene/node/MeshNode.h"
#include "scene/node/LightNode.h"

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
	}

	void RenderPipeline::onDetach(Ref<RenderContext>& context)
	{
	}

	void RenderPipeline::onRender(Ref<RenderContext>& context, std::vector<Ref<CameraNode>>& camNodes)
	{
		onBeginFrameRendering(context);		

		for (int32_t i = 0; i < camNodes.size(); i++)
		{					
			camNodes[i]->update(context);
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
		auto renderer = context->getRenderer();

		onInitializeRenderingData(context, camNode);

		renderer->onSetUp();

		renderer->onExecute(context, camNode);
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
		auto uBuffer = camNode->getRenderingData<CameraNode, Ref<UniformBuffer>>();
		renderingData.UnimBuffSet.insert({ uBuffer->binding(), uBuffer->assetHnd() });

		onSetUpLight(context, camNode, renderingNodes, renderingData);

		onSetUpObject(context, camNode, renderingNodes, renderingData);

		context->setRenderingData(std::move(renderingData));
	}

	void RenderPipeline::onSetUpLight(Ref<RenderContext>& context, Ref<CameraNode>& camNode, RenderingNodes& renderingNodes, RenderingData& renderingData)
	{
		auto& mainLightNode = renderingNodes.MainLight;
		mainLightNode->update(context, camNode);
		auto uBuffer = mainLightNode->getRenderingData<DirectionalLightNode, Ref<UniformBuffer>>();
		renderingData.UnimBuffSet.insert({ uBuffer->binding(), uBuffer->assetHnd() });
	}

	void RenderPipeline::onSetUpObject(Ref<RenderContext>& context, Ref<CameraNode>& camNode, RenderingNodes& renderingNodes, RenderingData &renderingData)
	{
		auto &meshNodes = renderingNodes.Mesh;
		for (auto &mesh : meshNodes)
		{
			mesh->update(context);
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