#ifndef __PIONNER_CORE_FUNC_DRAWABLE_H__
#define __PIONNER_CORE_FUNC_DRAWABLE_H__

#include "gfx/renderer/RenderData.h"

namespace pio
{
	class Drawable
	{
	public:
		Drawable() {}
		virtual ~Drawable() = default;

		virtual void onCreateMesh() = 0;
		virtual void onDraw(const DrawParam &param) = 0;
	};
}

#endif