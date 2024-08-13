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
		void beginFrameRendering();
		void endFrameRendering();

		void sortCameras();
		void beginCameraRendering();
		void renderSingleCamera();
		void endCameraRendering();
	};
}

#endif