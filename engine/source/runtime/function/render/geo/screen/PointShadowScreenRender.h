#ifndef __RENDER_GEO_POINT_SHAODW_SCREEN_RENDER_H__
#define __RENDER_GEO_POINT_SHAODW_SCREEN_RENDER_H__

#include "ScreenRender.h"

namespace Pionner
{
	class PointShadowScreenRender : public ScreenRender
	{
	public:
		PointShadowScreenRender();
		virtual ~PointShadowScreenRender();

		virtual void draw(RenderParam &param) override;

	protected:
		virtual bool dealShader(RenderParam &param, std::shared_ptr<Shader> &shader) override;
	};
}

#endif