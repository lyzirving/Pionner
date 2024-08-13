#include "RenderPipeline.h"

namespace pio
{
	void RenderPipeline::onRender()
	{
		beforeRendering();
		renderOpaque();
		renderTranparent();
		postRendering();
	}

	void RenderPipeline::beforeRendering()
	{
	}

	void RenderPipeline::renderOpaque()
	{
	}

	void RenderPipeline::renderTranparent()
	{
	}

	void RenderPipeline::postRendering()
	{
	}
}