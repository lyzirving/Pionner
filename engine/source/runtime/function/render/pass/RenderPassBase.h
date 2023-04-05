#ifndef __RENDER_PASS_BASE_H__
#define __RENDER_PASS_BASE_H__

#include "function/render/RenderDef.h"

namespace Pionner
{
	class RenderPassBase
	{
	public:
		RenderPassBase();
		virtual ~RenderPassBase();

		virtual void initialize(const RenderPassInitInfo &info) = 0;
		virtual void shutdown() = 0;

		virtual void initializeUIRenderBackend(WindowUI* ui);
		virtual void shutdownUIRenderBackend();
		virtual void draw();
		virtual void setPassCommonInfo(RenderPassCommonInfo &info);

	protected:
		std::shared_ptr<Rhi> m_rhi;
	};
}

#endif