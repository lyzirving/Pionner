#ifndef __PIONNER_GFX_PIPELINE_RENDERPIPELINE_H__
#define __PIONNER_GFX_PIPELINE_RENDERPIPELINE_H__

#include "Entry.h"

namespace pio
{
	class Camera;
	class RenderContext;
	class Renderer;
	struct RenderingEntities;
	struct RenderingData;

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
		void onInitializeRenderingData(Ref<RenderContext>& context, Ref<Camera>& camera, RenderingEntities& renderingEntities);
		void onSetUpCamera(Ref<RenderContext>& context, Ref<Camera>& camera, RenderingData &renderingData);
		void onSetUpLight(Ref<RenderContext>& context, RenderingEntities& renderingEntities, RenderingData &renderingData);
		void onSetUpObject(Ref<RenderContext>& context, RenderingEntities& renderingEntities, RenderingData &renderingData);

	protected:
		Ref<Renderer> m_renderer;
	};
}

#endif