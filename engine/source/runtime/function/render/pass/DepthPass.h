#ifndef __RENDER_PASS_DEPTH_PASS_H__
#define __RENDER_PASS_DEPTH_PASS_H__

#include "RenderPassBase.h"

namespace Pionner
{
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
		std::shared_ptr<ScreenRender>   m_depthScreenRender;
		std::shared_ptr<ScreenRender>   m_ptShadowScreenRender;
	};
}

#endif