#ifndef __PIONNER_GFX_RHI_FRAME_BUFFER_H__
#define __PIONNER_GFX_RHI_FRAME_BUFFER_H__

#include "Texture.h"

namespace pio
{
	struct FrameBufferSpecific
	{
		FrameBufferUsage Usage{ FrameBufferUsage::ColorBuffer };
		std::string Name;
		uint32_t Width{ 0 }, Height{ 0 };

		std::vector<TextureSpecific> ColorSpec;
		TextureSpecific DepthSpec;
	};

	class RenderContext;

	class FrameBuffer : public RenderResource
	{
	public:
		FrameBuffer(Ref<RenderContext>& context, const FrameBufferSpecific& spec) : RenderResource(context, RenderResourceType::FBO), m_spec(spec) {}
		virtual ~FrameBuffer() = default;

		virtual Ref<Texture> attach(FrameBufferAttach slot) = 0;

		const FrameBufferSpecific& spec() const { return m_spec; }
		uint32_t width() const { return spec().Width; }
		uint32_t height() const { return spec().Height; }
		const std::string& name() const { spec().Name; }

	protected:
		FrameBufferSpecific m_spec;

	public:
		static Ref<FrameBuffer> Create(Ref<RenderContext>& context, const FrameBufferSpecific& spec);

		static bool bAsColorBuffer(const FrameBuffer& fbo) { return fbo.spec().Usage == FrameBufferUsage::ColorBuffer; };
		static bool bAsDepthBuffer(const FrameBuffer& fbo) { return fbo.spec().Usage == FrameBufferUsage::DepthBuffer; };
		static bool bAsDepthStencil(const FrameBuffer& fbo) { return fbo.spec().Usage == FrameBufferUsage::DepthStencil; }
	};
}

#endif