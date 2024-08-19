#ifndef __PIONNER_GFX_RENDERER_RENDERER_H__
#define __PIONNER_GFX_RENDERER_RENDERER_H__

#include "GlobalSettings.h"

namespace pio
{
	class RenderContext;
	struct PendingData;

	class Renderer
	{
	public:
		Renderer() {}
		virtual ~Renderer() = default;

		virtual void onSetUp(PendingData &pendingData) = 0;
		virtual void onExecute(Ref<RenderContext> &context) = 0;

	public:
		static Ref<Renderer> Create(const RenderConfig &config);
	};
}

#endif