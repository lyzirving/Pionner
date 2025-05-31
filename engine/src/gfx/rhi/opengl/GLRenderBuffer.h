#pragma once

#include "gfx/rhi/Texture.h"

namespace pio
{
	class GLRenderBuffer : public RenderBuffer
	{
		RTTR_ENABLE(RenderBuffer)
	public:
		GLRenderBuffer(const Ref<RenderContext>& context, const TextureSpecific& spec);
		~GLRenderBuffer() = default;

		virtual bool Init() override;
		virtual void Destroy() override;
		virtual bool IsInit() const { return m_ID != 0; }

		virtual void Bind() override;
		virtual void Bind(uint32_t width, uint32_t height) override;
		virtual void UnBind() override;

		virtual uint32_t Size() const override { return m_Size; }
	};
}