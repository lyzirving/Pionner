#pragma once

#include "gfx/rhi/RenderAPI.h"

namespace pio
{
	class GLRenderAPI : public RenderAPI
	{
		RTTR_ENABLE(RenderAPI)
	public:
		GLRenderAPI(RenderBackendFlags flag);
		~GLRenderAPI() = default;

		virtual bool SetupBackend() override;
		virtual bool SetupUiBackend(void* nativeWindow = nullptr) override;
		virtual void Shutdown() override;

		virtual void SetViewport(int32_t x, int32_t y, int32_t w, int32_t h) override;
		virtual bool BindUnimBlock(uint32_t program, uint32_t bindingPt, const std::string& blockName) override;
		virtual void BindScreenFrameBuffer() override;
		virtual void DrawElements(DrawMode mode, uint32_t indexNum, IndexInternalFmt fmt) override;

		virtual void OnBeginFrame(RenderContext& ctx) override;
		virtual void OnEndFrame(RenderContext& ctx) override;

		virtual void ReleaseResource(ResourceGCDelegate &&resource) override;

	private:
		bool m_BackendSetup{ false }, m_UiSetup{ false };
	};
}