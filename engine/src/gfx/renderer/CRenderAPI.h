#ifndef __PIONNER_GFX_RENDERER_CRENDER_API_H__
#define __PIONNER_GFX_RENDERER_CRENDER_API_H__

#include "gfx/GfxDef.h"

namespace pio
{
	class RenderContext;
	class CRenderAPI
	{
	public:
		virtual ~CRenderAPI() = default;

		virtual bool setupBackend() = 0;
		virtual bool setupUiBackend(void* nativeWindow = nullptr) = 0;
		virtual void shutdown() = 0;

		virtual void beginFrame(RenderContext& ctx) = 0;
		virtual void endFrame(RenderContext& ctx) = 0;

		virtual BackendFlags type() const = 0;

	public:
		static Ref<CRenderAPI> Create(BackendFlags type);
	};
}

#endif