#include "RenderPipeline.h"
#include "RenderPipeline.h"
#include "RenderPipeline.h"
#include "RenderPipeline.h"
#include "RenderPipeline.h"
#include "RenderPipeline.h"
#include "RenderPipeline.h"

namespace pio
{
	void RenderPipeline::onRender()
	{
		beginFrameRendering();

		sortCameras();

		beginCameraRendering();
		renderSingleCamera();
		endCameraRendering();

		endFrameRendering();
	}

	void RenderPipeline::beginFrameRendering()
	{
	}

	void pio::RenderPipeline::endFrameRendering()
	{
	}

	void pio::RenderPipeline::sortCameras()
	{
	}

	void pio::RenderPipeline::beginCameraRendering()
	{
	}

	void pio::RenderPipeline::renderSingleCamera()
	{
	}

	void pio::RenderPipeline::endCameraRendering()
	{
	}
}