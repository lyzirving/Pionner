#include "RenderPipeline.h"
#include "PipelineUtils.h"

#include "scene/3d/Camera.h"

#include "gfx/renderer/RenderContext.h"
#include "gfx/renderer/Renderer.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "RenderPipeline"

namespace pio
{
	bool CameraSorter(Ref<Camera>& lhs, Ref<Camera>& rhs)
	{
		return lhs->depth() < rhs->depth();
	}

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

	void RenderPipeline::onRender(Ref<RenderContext>& context, std::vector<Ref<Camera>>& cameras)
	{
		if (cameras.empty())
		{
			LOGE("err! empty camera list");
		}

		onBeginFrameRendering(context);

		onSortCameras(cameras);

		for (int32_t i = 0; i < cameras.size(); i++)
		{
			onBeginCameraRendering(context, cameras[i]);
			onRenderSingleCamera(context, cameras[i]);
			onEndCameraRendering(context, cameras[i]);
		}

		onEndFrameRendering(context);
	}

	void RenderPipeline::onBeginFrameRendering(Ref<RenderContext>& context)
	{
	}

	void RenderPipeline::onEndFrameRendering(Ref<RenderContext>& context)
	{
	}

	void pio::RenderPipeline::onSortCameras(std::vector<Ref<Camera>>& cameras)
	{
		if (cameras.size() >= 2)
			std::sort(cameras.begin(), cameras.end(), CameraSorter);
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

		m_renderer->onExecute(context);
	}

	void RenderPipeline::onEndCameraRendering(Ref<RenderContext>& context, Ref<Camera>& camera)
	{
		context->onEndFrameRendering();
	}

	void RenderPipeline::onInitializeRenderingData(Ref<RenderContext>& context, Ref<Camera>& camera, RenderingEntities& renderingEntities)
	{
		RenderingData renderingData;

		onSetUpCamera(context, camera, renderingData);

		onSetUpLight(context, renderingEntities, renderingData);

		onSetUpObject(context, renderingEntities, renderingData);

		context->setRenderingData(std::move(renderingData));
	}

	void RenderPipeline::onSetUpCamera(Ref<RenderContext>& context, Ref<Camera>& camera, RenderingData &renderingData)
	{
		camera->setUp(context);
		auto &camBuff = camera->unimBuffer();
		renderingData.UnimBuffSet.insert({ camBuff->binding(), camBuff->assetHnd() });
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