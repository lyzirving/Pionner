#ifndef __PIONNER_GFX_RHI_RENDER_API_H__
#define __PIONNER_GFX_RHI_RENDER_API_H__

#include "RhiDef.h"

namespace pio
{
	class RenderContext;
	struct ResourceGCDelegate;

	class RenderAPI
	{
	public:
		virtual ~RenderAPI() = default;

		virtual bool setupBackend() = 0;
		virtual bool setupUiBackend(void* nativeWindow = nullptr) = 0;
		virtual void shutdown() = 0;

		virtual void beginFrame(RenderContext& ctx) = 0;
		virtual void endFrame(RenderContext& ctx) = 0;

		virtual void releaseResource(ResourceGCDelegate &&resource) = 0;

		virtual BackendFlags type() const = 0;

	public:
		static Ref<RenderAPI> Create(BackendFlags type);
	};
}

#endif