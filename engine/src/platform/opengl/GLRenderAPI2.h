#ifndef __PIONNER_PLATFORM_OPENGL_GL_RENDER_API_2_H__
#define __PIONNER_PLATFORM_OPENGL_GL_RENDER_API_2_H__

#include "gfx/renderer/CRenderAPI.h"

namespace pio
{
	class GLRenderAPI2 : public CRenderAPI
	{
	public:
		GLRenderAPI2(BackendFlags type);
		~GLRenderAPI2() = default;

		virtual bool setupBackend() override;
		virtual bool setupUiBackend(void* nativeWindow = nullptr) override;
		virtual void shutdown() override;

		virtual void beginFrame(RenderContext& ctx) override;
		virtual void endFrame(RenderContext& ctx) override;

		virtual BackendFlags type() const override { return m_type; }

	private:
		const BackendFlags m_type;
		bool m_bBackend{false}, m_bUiBackend{ false };
	};
}

#endif