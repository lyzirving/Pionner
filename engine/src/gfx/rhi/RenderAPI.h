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
		RenderAPI(RenderBackendFlags flag) : m_renderBackend(flag) {}
		virtual ~RenderAPI() = default;

		virtual bool setupBackend() = 0;
		virtual bool setupUiBackend(void* nativeWindow = nullptr) = 0;
		virtual void shutdown() = 0;

		virtual void beginFrame(RenderContext& ctx) = 0;
		virtual void endFrame(RenderContext& ctx) = 0;

		virtual void releaseResource(ResourceGCDelegate &&resource) = 0;

		RenderBackendFlags renderBackend() const { return m_renderBackend; }

	protected:
		const RenderBackendFlags m_renderBackend;

	public:
		static Ref<RenderAPI> Create(RenderBackendFlags flag);
	};
}

#endif