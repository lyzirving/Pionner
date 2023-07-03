#ifndef __LEFT_PANEL_VIEW_H__
#define __LEFT_PANEL_VIEW_H__

#include "window/ui/WindowView.h"

namespace pio
{
	class LeftPanelView : public WindowView
	{
	public:
		LeftPanelView();
		virtual ~LeftPanelView();

		virtual void draw(RenderParam &param) override;
		virtual void layout(int32_t windowWidth, int32_t windowHeight) override;
	};
}

#endif