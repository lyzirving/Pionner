#include "RenderPipeline.h"

#include "scene/3d/Camera.h"
#include "gfx/renderer/RenderContext.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "RenderPipeline"

namespace pio
{
	void RenderPipeline::onRender(Ref<RenderContext>& context, std::vector<Ref<Camera>>& cameras)
	{
		beginFrameRendering(context);

		sortCameras(cameras);

		for (int32_t i = 0; i < cameras.size(); i++)
		{
			beginCameraRendering(context, cameras[i]);
			renderSingleCamera(context, cameras[i]);
			endCameraRendering(context, cameras[i]);
		}

		endFrameRendering(context);
	}

	void RenderPipeline::beginFrameRendering(Ref<RenderContext>& context)
	{
	}

	void RenderPipeline::endFrameRendering(Ref<RenderContext>& context)
	{
	}

	void pio::RenderPipeline::sortCameras(std::vector<Ref<Camera>>& cameras)
	{
	}

	void RenderPipeline::beginCameraRendering(Ref<RenderContext>& context, Ref<Camera>& camera)
	{
	}

	void RenderPipeline::renderSingleCamera(Ref<RenderContext>& context, Ref<Camera>& camera)
	{
	}

	void RenderPipeline::endCameraRendering(Ref<RenderContext>& context, Ref<Camera>& camera)
	{
	}
}