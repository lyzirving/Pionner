#ifndef __RENDER_VISUAL_ANGLE_VIEW_H__
#define __RENDER_VISUAL_ANGLE_VIEW_H__

#include <vector>

#include "function/ui/WindowView.h"

namespace Pionner
{
	class  CoordinateAxis;
	class  Sphere;

	class VisualAngleView : public WindowView
	{
	public:
		VisualAngleView();
		virtual ~VisualAngleView();

		virtual void draw(RenderParam &param) override;
		virtual void layout(int32_t windowWidth, int32_t windowHeight) override;
		virtual bool processEvent(RenderParam &param, const Event &evt) override;

	private:
		std::shared_ptr<CoordinateAxis> m_coordinateAxis;
		std::shared_ptr<Sphere>         m_sphere;
		ViewLayout                      m_renderport;
	};
}

#endif