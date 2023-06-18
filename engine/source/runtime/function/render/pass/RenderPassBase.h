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
		virtual void draw(RenderParam &param) = 0;

		inline const RenderViewport &getViewport() { return m_viewport; }
		inline void setViewport(uint32_t left, uint32_t top, uint32_t width, uint32_t height)
		{
			m_viewport.m_left = left; m_viewport.m_top = top;
			m_viewport.m_width = width; m_viewport.m_height = height;
		}

	protected:
		std::shared_ptr<Rhi> m_rhi;
		RenderViewport       m_viewport;
	};
}

#endif