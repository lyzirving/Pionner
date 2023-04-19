#ifndef __RENDER_SYSTEM_H__
#define __RENDER_SYSTEM_H__

#include "function/render/RenderDef.h"

namespace Pionner
{
	struct SwapData;
	class  RenderScene;
	class  Camera;
	class  Frustum;

	class RenderSystem
	{
	public:
		RenderSystem();
		~RenderSystem();

		void initialize(RenderSystemInitInfo &info);
		void initializeUIRenderBackend(WindowUI *windowUI);
		void shutdownUIRenderBackend();
		void shutdown();

		void processSwapData(const SwapData &data);

		void tick(float delta);

	private:
		std::shared_ptr<Rhi>                m_rhi;
		std::shared_ptr<RenderPipelineBase> m_pipeLine;
		std::shared_ptr<RenderScene>        m_scene;
		std::shared_ptr<Camera>             m_camera;
		std::shared_ptr<Frustum>            m_frustum;
	};
}

#endif