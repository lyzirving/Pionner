#include "RenderPipeline.h"
#include "PipelineUtils.h"

#include "asset/AssetMgr.h"

#include "scene/Entity.h"
#include "scene/Components.h"
#include "scene/3d/Camera.h"

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

	void RenderPipeline::onRender(Ref<RenderContext>& context, std::vector<Ref<Entity>>& cameras)
	{
		onBeginFrameRendering(context);		

		for (int32_t i = 0; i < cameras.size(); i++)
		{			
			auto camera = AssetMgr::GetRuntimeAsset<Camera>(cameras[i]->getComponent<CameraComponent>()->Uid);
			Pipeline::UpdateCamera(context, cameras[i], camera);
			onBeginCameraRendering(context, camera);
			onRenderSingleCamera(context, camera);
			onEndCameraRendering(context, camera);
		}

		onEndFrameRendering(context);
	}

	void RenderPipeline::onBeginFrameRendering(Ref<RenderContext>& context)
	{
	}

	void RenderPipeline::onEndFrameRendering(Ref<RenderContext>& context)
	{
	}

	void RenderPipeline::onBeginCameraRendering(Ref<RenderContext>& context, Ref<Camera>& camera)
	{
		context->onBeginFrameRendering();
	}

	void RenderPipeline::onRenderSingleCamera(Ref<RenderContext>& context, Ref<Camera>& camera)
	{
		auto& renderingEntities = context->renderingEntities();
		camera->culling(renderingEntities);

		onInitializeRenderingData(context, camera, renderingEntities);

		m_renderer->onSetUp();

		m_renderer->onExecute(context, camera);
	}

	void RenderPipeline::onEndCameraRendering(Ref<RenderContext>& context, Ref<Camera>& camera)
	{
		context->onEndFrameRendering();
	}

	void RenderPipeline::onInitializeRenderingData(Ref<RenderContext>& context, Ref<Camera>& camera, RenderingEntities& renderingEntities)
	{
		RenderingData renderingData;
		renderingData.UnimBuffSet.insert({ camera->unimBuffer()->binding(), camera->unimBuffer()->assetHnd()});

		onSetUpLight(context, renderingEntities, renderingData);

		onSetUpObject(context, renderingEntities, renderingData);

		context->setRenderingData(std::move(renderingData));
	}

	void RenderPipeline::onSetUpLight(Ref<RenderContext>& context, RenderingEntities& renderingEntities, RenderingData &renderingData)
	{
	}

	void RenderPipeline::onSetUpObject(Ref<RenderContext>& context, RenderingEntities& renderingEntities, RenderingData &renderingData)
	{
		auto &meshEnts = renderingEntities.Mesh;
		for (auto &ent : meshEnts)
		{
			Pipeline::ProcessMeshEnt(context, ent, renderingData);
		}
	}
}