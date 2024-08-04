#ifndef __PIONNER_GFX_STRUCT_RENDER_TARGET_H__
#define __PIONNER_GFX_STRUCT_RENDER_TARGET_H__

#include "core/Base.h"

namespace pio
{
	class RenderTarget
	{
	public:
		RenderTarget() {}
		~RenderTarget() = default;

		void setViewport(int32_t x, int32_t y, int32_t wid, int32_t height) { m_viewport.setX(x); m_viewport.setY(y); m_viewport.setW(wid); m_viewport.setH(height); }

		Viewport& viewport() { return m_viewport; }
		const Viewport& viewport() const { return m_viewport; }

	private:
		Viewport m_viewport{};
	};
}

#endif