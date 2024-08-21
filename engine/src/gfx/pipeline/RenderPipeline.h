#ifndef __PIONNER_GFX_PIPELINE_RENDERPIPELINE_H__
#define __PIONNER_GFX_PIPELINE_RENDERPIPELINE_H__

#include "Base.h"

namespace pio
{
	class Camera;
	class RenderContext;
	class Renderer;
	struct PendingData;

	class RenderPipeline
	{
	public:
		RenderPipeline();
		~RenderPipeline() = default;

		virtual void onRender(Ref<RenderContext> &context, std::vector<Ref<Camera>>& cameras);

	protected:
		void onBeginFrameRendering(Ref<RenderContext>& context);
		void onEndFrameRendering(Ref<RenderContext>& context);

		void onSortCameras(std::vector<Ref<Camera>>& cameras);
		void onBeginCameraRendering(Ref<RenderContext>& context, Ref<Camera>& camera);
		void onRenderSingleCamera(Ref<RenderContext>& context, Ref<Camera>& camera);
		void onEndCameraRendering(Ref<RenderContext>& context, Ref<Camera>& camera);

		// Methods where make data  which is about to be uploaded
		void onInitializeRenderingData(Ref<RenderContext>& context, Ref<Camera>& camera, PendingData& pendingData);
		void onSetUpCamera(Ref<RenderContext>& context, Ref<Camera>& camera);
		void onSetUpLight(Ref<RenderContext>& context, PendingData& pendingData);
		void onSetUpObject(Ref<RenderContext>& context, PendingData& pendingData);

	protected:
		Ref<Renderer> m_renderer;
	};
}

#endif