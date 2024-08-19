#ifndef __PIONNER_GFX_RENDERER_RENDERER_H__
#define __PIONNER_GFX_RENDERER_RENDERER_H__

#include "GlobalSettings.h"

namespace pio
{
	class RenderContext;

	class Renderer
	{
	public:
		Renderer() {}
		virtual ~Renderer() = default;

		virtual void onSetUp() = 0;
		virtual void onRender(Ref<RenderContext> &context) = 0;

	public:
		static Ref<Renderer> Create(const RenderConfig &config);
	};
}

#endif