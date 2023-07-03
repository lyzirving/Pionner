#ifndef __BOTTOM_PANEL_VIEW_H__
#define __BOTTOM_PANEL_VIEW_H__

#include "window/ui/WindowView.h"

namespace pio
{
	class BottomPanelView : public WindowView
	{
	public:
		BottomPanelView();
		virtual ~BottomPanelView();

		virtual void draw(RenderParam &param) override;
		virtual void layout(int32_t windowWidth, int32_t windowHeight) override;
	};
}

#endif