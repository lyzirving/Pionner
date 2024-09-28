#ifndef __PIONNER_GFX_PIPELINE_RENDERPIPELINE_H__
#define __PIONNER_GFX_PIPELINE_RENDERPIPELINE_H__

#include "Common.h"

namespace pio
{
	class CameraNode;
	class RenderContext;
	struct RenderingNodes;
	struct RenderingData;

	class RenderPipeline
	{
	public:
		RenderPipeline();
		~RenderPipeline() = default;

		virtual void onAttach(Ref<RenderContext>& context);
		virtual void onDetach(Ref<RenderContext>& context);
		virtual void onRender(Ref<RenderContext>& context, std::vector<Ref<CameraNode>>& camNodes);

	protected:
		void onBeginFrameRendering(Ref<RenderContext>& context);
		void onEndFrameRendering(Ref<RenderContext>& context);

		void onBeginCameraRendering(Ref<RenderContext>& context, Ref<CameraNode>& camNode);
		void onRenderSingleCamera(Ref<RenderContext>& context, Ref<CameraNode>& camNode);
		void onEndCameraRendering(Ref<RenderContext>& context, Ref<CameraNode>& camNode);

		// Methods where make data  which is about to be uploaded
		void onInitializeRenderingData(Ref<RenderContext>& context, Ref<CameraNode>& camNode);
		void onSetupRenderingData(Ref<RenderContext>& context, Ref<CameraNode>& camNode, RenderingNodes& renderingNodes, RenderingData& renderingData);		
	};
}

#endif