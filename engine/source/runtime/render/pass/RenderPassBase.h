#ifndef __RENDER_PASS_BASE_H__
#define __RENDER_PASS_BASE_H__

#include "render/RenderDef.h"

namespace pio
{
	class RenderPassBase
	{
	public:
		RenderPassBase();
		virtual ~RenderPassBase();

		virtual void initialize(const RenderPassInitInfo &info) = 0;
		virtual void shutdown() = 0;
		virtual void draw(RenderParam &param) = 0;

	protected:
		std::shared_ptr<Rhi> m_rhi;
	};
}

#endif