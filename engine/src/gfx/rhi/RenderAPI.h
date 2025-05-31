#pragma once

#include "gfx/GfxDef.h"

namespace pio
{
	class RenderContext;
	struct ResourceGCDelegate;

	class RenderAPI : public Object
	{
		RTTR_ENABLE(Object)
	public:
		RenderAPI(RenderBackendFlags flag) : m_RenderBackend(flag) {}
		virtual ~RenderAPI() = default;

		virtual bool SetupBackend() = 0;
		virtual bool SetupUiBackend(void* nativeWindow = nullptr) = 0;
		virtual void Shutdown() = 0;

		virtual void SetViewport(int32_t x, int32_t y, int32_t w, int32_t h) = 0;
		virtual bool BindUnimBlock(uint32_t program, uint32_t bindingPt, const std::string& blockName) = 0;
		virtual void BindScreenFrameBuffer() = 0;
		virtual void DrawElements(DrawMode mode, uint32_t indexNum, IndexInternalFmt fmt) = 0;

		virtual void OnBeginFrame(RenderContext& ctx) = 0;
		virtual void OnEndFrame(RenderContext& ctx) = 0;		

		virtual void ReleaseResource(ResourceGCDelegate &&resource) = 0;

		RenderBackendFlags RenderBackend() const { return m_RenderBackend; }

	protected:
		const RenderBackendFlags m_RenderBackend;

	public:
		static Ref<RenderAPI> Create(RenderBackendFlags flag);
	};
}