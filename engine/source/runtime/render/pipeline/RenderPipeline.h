#ifndef __RENDER_PIPELINE_H__
#define __RENDER_PIPELINE_H__

#include "render/RenderDef.h"

namespace pio
{
	class Event;
	class DepthPass;
	class UIPass;

	class RenderPipeline
	{
		friend class RenderSystem;

	public:
		RenderPipeline();
		virtual ~RenderPipeline();

		virtual void initialize(RenderPipelineInitInfo &info);
		virtual void initializeUIRenderBackend(const std::shared_ptr<WindowUI> &ui);

		virtual void shutdownUIRenderBackend();
		virtual void shutdown();

		virtual void forwardRender(RenderParam &param);
		virtual void preparePassData(RenderParam &param);
		virtual void swapData(RenderParam &param);

	protected:
		std::shared_ptr<Rhi>    m_rhi;
		std::shared_ptr<UIPass> m_uiPass;

	private:
		std::shared_ptr<DepthPass> m_depthPass;
	};
}

#endif