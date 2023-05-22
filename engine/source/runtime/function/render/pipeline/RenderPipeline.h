#ifndef __RENDER_PIPELINE_H__
#define __RENDER_PIPELINE_H__

#include "function/render/pipeline/RenderPipelineBase.h"

namespace Pionner
{
	class RenderPipeline : public RenderPipelineBase
	{
	public:
		RenderPipeline();
		virtual ~RenderPipeline();

		virtual void initialize(RenderPipelineInitInfo &info) override;
		virtual void shutdownUIRenderBackend() override;
		virtual void shutdown() override;

		virtual void forwardRender(RenderParam &param) override;
	};
}

#endif