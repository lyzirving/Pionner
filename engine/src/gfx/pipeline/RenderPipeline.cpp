#include "RenderPipeline.h"

#include "scene/3d/Camera.h"

#include "gfx/renderer/RenderContext.h"
#include "gfx/renderer/Renderer.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "RenderPipeline"

namespace pio
{
	bool CameraSorter(Ref<Camera> &lhs, Ref<Camera> &rhs)
	{
		return lhs->depth() < rhs->depth();
	}

	RenderPipeline::RenderPipeline()
	{
		m_renderer = Renderer::Create(GlobalSettings::k_RenderConfig);
	}

	void RenderPipeline::onRender(Ref<RenderContext>& context, std::vector<Ref<Camera>>& cameras)
	{
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
	}

	void RenderPipeline::onRenderSingleCamera(Ref<RenderContext>& context, Ref<Camera>& camera)
	{
		camera->makeCulling();

		m_renderer->onSetUp();

		m_renderer->onRender(context);
	}

	void RenderPipeline::onEndCameraRendering(Ref<RenderContext>& context, Ref<Camera>& camera)
	{
	}
}