#ifndef __RENDER_PIPELINE_BASE_H__
#define __RENDER_PIPELINE_BASE_H__

#include "function/render/RenderDef.h"

namespace Pionner
{
	class RenderScene;

	class RenderPipelineBase
	{
		friend class RenderSystem;

	public:
		RenderPipelineBase();
		virtual ~RenderPipelineBase();

		virtual void initialize(RenderPipelineInitInfo &info) = 0;
		virtual void shutdown() = 0;

		virtual void forwardRender(const std::shared_ptr<RenderScene> &scene, const RenderParam &param);
		virtual void initializeUIRenderBackend(WindowUI *ui);
		virtual void shutdownUIRenderBackend();
		virtual void preparePassData();

	protected:
		std::shared_ptr<Rhi> m_rhi;

		std::shared_ptr<RenderPassBase> m_uiPass;
		std::shared_ptr<RenderPassBase> m_mainCameraPass;
	};
}

#endif