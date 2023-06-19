#ifndef __RENDER_PASS_DEPTH_PASS_H__
#define __RENDER_PASS_DEPTH_PASS_H__

#include "RenderPassBase.h"

namespace Pionner
{
	class GfxFrameBuffer;
	class ScreenRender;

	class DepthPass : public RenderPassBase
	{
	public:
		DepthPass();
		virtual ~DepthPass();

		virtual void initialize(const RenderPassInitInfo &info) override;
		virtual void shutdown() override;
		virtual void draw(RenderParam &param) override;

	private:
		uint32_t m_depthFboWidth, m_depthFboHeight;
		std::shared_ptr<GfxFrameBuffer> m_depthFbo;
		std::shared_ptr<ScreenRender>   m_depthScreenRender;
	};
}

#endif