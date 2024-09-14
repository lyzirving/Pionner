#ifndef __PIONNER_GFX_PIPELINE_RENDERPIPELINE_H__
#define __PIONNER_GFX_PIPELINE_RENDERPIPELINE_H__

#include "Common.h"

namespace pio
{
	class Entity;
	class Camera;
	class Renderer;
	class RenderContext;
	struct RenderingEntities;
	struct RenderingData;

	class RenderPipeline
	{
	public:
		RenderPipeline();
		~RenderPipeline() = default;

		virtual void onAttach(Ref<RenderContext>& context);
		virtual void onDetach(Ref<RenderContext>& context);
		virtual void onRender(Ref<RenderContext>& context, std::vector<Ref<Entity>>& cameras);

	protected:
		void onBeginFrameRendering(Ref<RenderContext>& context);
		void onEndFrameRendering(Ref<RenderContext>& context);

		void onBeginCameraRendering(Ref<RenderContext>& context, Ref<Camera>& camera);
		void onRenderSingleCamera(Ref<RenderContext>& context, Ref<Camera>& camera);
		void onEndCameraRendering(Ref<RenderContext>& context, Ref<Camera>& camera);

		// Methods where make data  which is about to be uploaded
		void onInitializeRenderingData(Ref<RenderContext>& context, Ref<Camera>& camera, RenderingEntities& renderingEntities);
		void onSetUpLight(Ref<RenderContext>& context, RenderingEntities& renderingEntities, RenderingData &renderingData);
		void onSetUpObject(Ref<RenderContext>& context, RenderingEntities& renderingEntities, RenderingData &renderingData);

	protected:
		Ref<Renderer> m_renderer;
	};
}

#endif