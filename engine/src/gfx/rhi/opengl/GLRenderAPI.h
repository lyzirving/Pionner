#ifndef __PIONNER_GFX_RHI_OPENGL_GL_RENDER_API_H__
#define __PIONNER_GFX_RHI_OPENGL_GL_RENDER_API_H__

#include "gfx/rhi/RenderAPI.h"

namespace pio
{
	class GLRenderAPI : public RenderAPI
	{
	public:
		GLRenderAPI(RenderBackendFlags flag);
		~GLRenderAPI() = default;

		virtual bool setupBackend() override;
		virtual bool setupUiBackend(void* nativeWindow = nullptr) override;
		virtual void shutdown() override;

		virtual void setViewport(int32_t x, int32_t y, int32_t w, int32_t h) override;
		virtual bool bindUnimBlock(uint32_t program, uint32_t bindingPt, const std::string& blockName) override;
		virtual void drawElements(DrawMode mode, uint32_t indexNum) override;

		virtual void onBeginFrame(RenderContext& ctx) override;
		virtual void onEndFrame(RenderContext& ctx) override;

		virtual void releaseResource(ResourceGCDelegate &&resource) override;

	private:
		bool m_backendSetup{false}, m_uiSetup{ false };
	};
}

#endif