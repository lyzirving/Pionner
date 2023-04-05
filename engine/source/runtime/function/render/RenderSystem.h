#ifndef __RENDER_SYSTEM_H__
#define __RENDER_SYSTEM_H__

#include "function/render/RenderDef.h"

namespace Pionner
{
	class RenderSystem
	{
	public:
		RenderSystem();
		~RenderSystem();

		void initialize(RenderSystemInitInfo &info);
		void initializeUIRenderBackend(WindowUI *windowUI);
		void shutdownUIRenderBackend();
		void shutdown();

		void tick(float delta);

	private:
		std::shared_ptr<Rhi> m_rhi;

		std::shared_ptr<RenderPipelineBase> m_pipeLine;
	};
}

#endif