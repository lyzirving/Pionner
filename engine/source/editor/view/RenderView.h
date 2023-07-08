#ifndef __RENDER_VIEW_H__
#define __RENDER_VIEW_H__

#include "global/window/ui/WindowView.h"

namespace pio
{
	class RenderView : public WindowView
	{
	public:
		RenderView();
		virtual ~RenderView();

		virtual void draw(RenderParam &param) override;
		virtual void layout(int32_t windowWidth, int32_t windowHeight) override;
	};
}

#endif