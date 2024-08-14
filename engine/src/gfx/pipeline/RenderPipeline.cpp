#include "RenderPipeline.h"

#include "gfx/core/Camera.h"
#include "gfx/renderer/RenderContext.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "RenderPipeline"

namespace pio
{
	void RenderPipeline::onRender(Ref<RenderContext>& ctx, std::vector<Ref<Camera>>& cameras)
	{
		beginFrameRendering(ctx);

		sortCameras(cameras);

		for (int32_t i = 0; i < cameras.size(); i++)
		{
			beginCameraRendering(ctx, cameras[i]);
			renderSingleCamera(ctx, cameras[i]);
			endCameraRendering(ctx, cameras[i]);
		}

		endFrameRendering(ctx);
	}

	void RenderPipeline::beginFrameRendering(Ref<RenderContext>& ctx)
	{
	}

	void RenderPipeline::endFrameRendering(Ref<RenderContext>& ctx)
	{
	}

	void pio::RenderPipeline::sortCameras(std::vector<Ref<Camera>>& cameras)
	{
	}

	void RenderPipeline::beginCameraRendering(Ref<RenderContext>& ctx, Ref<Camera>& camera)
	{
	}

	void RenderPipeline::renderSingleCamera(Ref<RenderContext>& ctx, Ref<Camera>& camera)
	{
	}

	void RenderPipeline::endCameraRendering(Ref<RenderContext>& ctx, Ref<Camera>& camera)
	{
	}
}