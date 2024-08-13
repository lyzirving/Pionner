#ifndef __PIONNER_GFX_PIPELINE_RENDERPIPELINE_H__
#define __PIONNER_GFX_PIPELINE_RENDERPIPELINE_H__

#include "core/Base.h"

namespace pio 
{
	class RenderPipeline
	{
	public:
		RenderPipeline() {}
		~RenderPipeline() = default;

		virtual void onRender();

	protected:
		virtual void beforeRendering();
		virtual void renderOpaque();
		virtual void renderTranparent();
		virtual void postRendering();
	};
}

#endif