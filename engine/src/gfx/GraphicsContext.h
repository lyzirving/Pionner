#ifndef __PIONNER_GFX_GRAPHICS_CONTEXT_H__
#define __PIONNER_GFX_GRAPHICS_CONTEXT_H__

#include "core/Base.h"

namespace pio
{
	class GraphicsContext
	{
	public:
		GraphicsContext() {}
		virtual ~GraphicsContext() = default;

		virtual void init() = 0;
		virtual void swapBuffer() = 0;

	public:
		static Scope<GraphicsContext> create(void *window);
	};
}

#endif