#ifndef __RENDER_LAYER_H__
#define __RENDER_LAYER_H__

#include <memory>

namespace pio
{
	struct RenderParam;

	class RenderLayer
	{
	public:
		RenderLayer();
		virtual ~RenderLayer();

		virtual void draw(RenderParam &param) = 0;
	};
}

#endif