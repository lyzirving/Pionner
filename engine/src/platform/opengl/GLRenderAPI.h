#ifndef __PIONNER_PLATFORM_OPENGL_GL_RENDER_API_H__
#define __PIONNER_PLATFORM_OPENGL_GL_RENDER_API_H__

#include "gfx/rhi/RenderAPI.h"

namespace pio
{
	class GLRenderAPI : public RenderAPI
	{
	public:
		GLRenderAPI(BackendFlags type);
		~GLRenderAPI() = default;

		virtual bool setupBackend() override;
		virtual bool setupUiBackend(void* nativeWindow = nullptr) override;
		virtual void shutdown() override;

		virtual void beginFrame(RenderContext& ctx) override;
		virtual void endFrame(RenderContext& ctx) override;

		virtual BackendFlags type() const override { return m_type; }

	private:
		const BackendFlags m_type;
		bool m_backendSetup{false}, m_uiSetup{ false };
	};
}

#endif