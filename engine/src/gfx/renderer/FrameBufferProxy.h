#pragma once

#include "gfx/rhi/FrameBuffer.h"

namespace pio
{
	class FrameBufferProxy : public FrameBuffer
	{
		RTTR_ENABLE(FrameBuffer)
	public:
		FrameBufferProxy(const Ref<RenderContext>& context, const std::string& name);
		virtual ~FrameBufferProxy() = default;

		virtual bool Init() override { return m_FrameBuff->Init(); }
		virtual void Destroy() override { m_FrameBuff->Destroy(); }
		virtual bool IsInit() const { return m_FrameBuff->IsInit(); }

		virtual void Bind() override { m_FrameBuff->Bind(); }
		virtual void UnBind() override { m_FrameBuff->UnBind(); }

		virtual const FrameBufferSpecific& Spec() const override { return m_FrameBuff->Spec(); }
		virtual uint32_t Id() const override { return m_FrameBuff->Id(); }

		virtual const std::vector<Ref<Texture>>& ColorBuffers() const override { return m_FrameBuff->ColorBuffers(); }
		virtual const std::vector<Ref<Texture>>& DepthBuffers() const override { return m_FrameBuff->DepthBuffers(); }
		virtual const Ref<Texture>& DepthBuffer() const override { return m_FrameBuff->DepthBuffer(); };

		virtual void BindWritingDepth(uint8_t sel = 0) override { m_FrameBuff->BindWritingDepth(sel); };

	protected:
		Ref<FrameBuffer> m_FrameBuff;
	};
}