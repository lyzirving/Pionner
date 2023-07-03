#ifndef __RENDER_GEO_DEPTH_SCREEN_RENDER_H__
#define __RENDER_GEO_DEPTH_SCREEN_RENDER_H__

#include "ScreenRender.h"

namespace pio
{
	class DepthScreenRender : public ScreenRender
	{
	public:
		DepthScreenRender();
		virtual ~DepthScreenRender();

		virtual void draw(RenderParam &param) override;

	protected:
		virtual bool dealShader(RenderParam &param, std::shared_ptr<Shader> &shader) override;
	};
}

#endif