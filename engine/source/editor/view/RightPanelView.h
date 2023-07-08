#ifndef __RIGHT_PANEL_VIEW_H__
#define __RIGHT_PANEL_VIEW_H__

#include "global/window/ui/WindowView.h"

namespace pio
{
	class RightPanelView : public WindowView
	{
	public:
		RightPanelView();
		virtual ~RightPanelView();

		virtual void draw(RenderParam &param) override;
		virtual void layout(int32_t windowWidth, int32_t windowHeight) override;
	};
}

#endif