#pragma once

#include "gfx/rhi/FrameBuffer.h"

namespace pio
{
	class GLFrameBuffer : public FrameBuffer
	{
		RTTR_ENABLE(FrameBuffer)
	public:
		GLFrameBuffer(const Ref<RenderContext>& context, const FrameBufferSpecific& spec);
		~GLFrameBuffer() = default;

		virtual bool Init() override;
		virtual void Destroy() override;
		virtual bool IsInit() const { return m_ID != 0; }

		virtual void Bind() override;
		virtual void UnBind() override;

		virtual const std::vector<Ref<Texture>>& ColorBuffers() const override { return m_ColorBuffs; }
		virtual const std::vector<Ref<Texture>>& DepthBuffers() const override { return m_DepthBuffs; }
		virtual const Ref<Texture>& DepthBuffer() const override { return m_DepthBuffs[m_DepSel]; };

		virtual void BindWritingDepth(uint8_t sel = 0) override;

	protected:
		bool SetDepBinding(uint8_t sel);

	private:
		std::vector<Ref<Texture>> m_ColorBuffs;
		std::vector<Ref<Texture>> m_DepthBuffs;
		uint8_t m_DepSel{ 0 };
	};
}