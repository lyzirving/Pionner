#ifndef __RENDER_VISUAL_ANGLE_VIEW_H__
#define __RENDER_VISUAL_ANGLE_VIEW_H__

#include "function/ui/WindowView.h"

namespace Pionner
{
	class VisualAngleView : public WindowView
	{
	public:
		VisualAngleView();
		virtual ~VisualAngleView();

		virtual void draw(RenderParam &param) override;
		virtual void layout(int32_t windowWidth, int32_t windowHeight) override;
	};
}

#endif