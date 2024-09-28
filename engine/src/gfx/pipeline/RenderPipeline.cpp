#include "RenderPipeline.h"

#include "asset/AssetMgr.h"

#include "scene/Components.h"
#include "scene/3d/Camera.h"

#include "scene/node/CameraNode.h"
#include "scene/node/MeshNode.h"
#include "scene/node/LightNode.h"
#include "scene/node/SpriteNode.h"
#include "scene/node/GizmoNode.h"

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
	
		onSetupRenderingData(context, camNode, context->renderingNodes(), renderingData);

		context->setRenderingData(std::move(renderingData));
	}

	void RenderPipeline::onSetupRenderingData(Ref<RenderContext>& context, Ref<CameraNode>& camNode, RenderingNodes& renderingNodes, RenderingData& renderingData)
	{
		camNode->onUpdate(context, renderingData);
		camNode->camera()->culling(renderingNodes);
		//[NOTE] take care of the update order of each node vector
		renderingNodes.MainLight->onUpdate(context, camNode, renderingData);

		for (auto& mesh : renderingNodes.Mesh)
		{
			if (mesh)
				mesh->onUpdate(context, camNode, renderingData);
		}

		for (auto& sprite : renderingNodes.Sprite)
		{
			if (sprite)
				sprite->onUpdate(context, camNode, renderingData);
		}

		for (auto& gizmo : renderingNodes.Gizmo)
		{
			if (gizmo)
				gizmo->onUpdate(context, camNode, renderingData);
		}
	}
}