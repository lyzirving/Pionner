#ifndef __RENDER_PIPELINE_BASE_H__
#define __RENDER_PIPELINE_BASE_H__

#include "function/render/RenderDef.h"

namespace Pionner
{
	class UIPass;

	class RenderPipelineBase
	{
		friend class RenderSystem;

	public:
		RenderPipelineBase();
		virtual ~RenderPipelineBase();

		virtual void initialize(RenderPipelineInitInfo &info) = 0;
		virtual void shutdown() = 0;

		virtual void forwardRender(RenderParam &param);
		virtual void preparePassData(RenderParam &param);

		virtual void initializeUIRenderBackend(WindowUI *ui);
		virtual void shutdownUIRenderBackend();

	protected:
		std::shared_ptr<Rhi> m_rhi;

		std::shared_ptr<UIPass> m_uiPass;
	};
}

#endif