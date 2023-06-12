#ifndef __RENDER_PIPELINE_H__
#define __RENDER_PIPELINE_H__

#include "RenderPipelineBase.h"

namespace Pionner
{
	class Event;
	class DepthPass;

	class RenderPipeline : public RenderPipelineBase
	{
	public:
		RenderPipeline();
		virtual ~RenderPipeline();

		virtual void initialize(RenderPipelineInitInfo &info) override;
		virtual void shutdownUIRenderBackend() override;
		virtual void shutdown() override;

		virtual void forwardRender(RenderParam &param) override;
		virtual void preparePassData(RenderParam &param) override;

	private:
		std::shared_ptr<DepthPass> m_depthPass;
	};
}

#endif