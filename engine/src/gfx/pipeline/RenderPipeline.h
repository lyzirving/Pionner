#ifndef __PIONNER_GFX_PIPELINE_RENDERPIPELINE_H__
#define __PIONNER_GFX_PIPELINE_RENDERPIPELINE_H__

#include "Base.h"

namespace pio
{
	class Camera;
	class RenderContext;

	class RenderPipeline
	{
	public:
		RenderPipeline() {}
		~RenderPipeline() = default;

		virtual void onRender(Ref<RenderContext> &ctx, std::vector<Ref<Camera>>& cameras);

	protected:
		void beginFrameRendering(Ref<RenderContext>& ctx);
		void endFrameRendering(Ref<RenderContext>& ctx);

		void sortCameras(std::vector<Ref<Camera>>& cameras);
		void beginCameraRendering(Ref<RenderContext>& ctx, Ref<Camera>& camera);
		void renderSingleCamera(Ref<RenderContext>& ctx, Ref<Camera>& camera);
		void endCameraRendering(Ref<RenderContext>& ctx, Ref<Camera>& camera);
	};
}

#endif