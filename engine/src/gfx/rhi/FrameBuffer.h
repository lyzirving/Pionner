#ifndef __PIONNER_GFX_RHI_FRAME_BUFFER_H__
#define __PIONNER_GFX_RHI_FRAME_BUFFER_H__

#include "Texture.h"

namespace pio
{
	struct FrameBufferSpecific
	{
		std::string Name;
		uint32_t Usage{ FrameBufferUsage_Color };
		uint32_t Width{ 0 }, Height{ 0 };

		std::vector<TextureSpecific> ColorSpec;
		TextureSpecific DepthSpec;
	};

	#define PIO_FBO_ADD_USAGE(flags, usage)  (flags = (flags | (usage)))
	#define PIO_FBO_RMV_USAGE(flags, usage)  (flags = (flags & (~usage)))
	#define PIO_FBO_HAS_USAGE(flags, usage)  (flags & usage)

	#define PIO_FBO_IS_COLOR_BUF(flags)	PIO_FBO_HAS_USAGE(flags, FrameBufferUsage_Color)
	#define PIO_FBO_IS_DEPTH_BUF(flags)	PIO_FBO_HAS_USAGE(flags, FrameBufferUsage_Depth)
	#define PIO_FBO_IS_DEPTH_STENCIL(flags)	 (PIO_FBO_HAS_USAGE(flags, FrameBufferUsage_Depth) && \
											  PIO_FBO_HAS_USAGE(flags, FrameBufferUsage_Stencil))

	class RenderContext;

	class FrameBuffer : public RenderResource
	{
	public:
		FrameBuffer(Ref<RenderContext>& context, const FrameBufferSpecific& spec) : RenderResource(context, RenderResourceType::FBO, spec.Name), m_spec(spec) {}
		virtual ~FrameBuffer() = default;

		virtual const std::vector<Ref<Texture>>& colorBuffers() const = 0;
		virtual const Ref<Texture>& depthBuffer() const = 0;

		const FrameBufferSpecific& spec() const { return m_spec; }

		uint32_t width() const { return spec().Width; }
		uint32_t height() const { return spec().Height; }

	protected:
		const FrameBufferSpecific m_spec;

	public:
		static Ref<FrameBuffer> Create(Ref<RenderContext>& context, const FrameBufferSpecific& spec);
	};
}

#endif