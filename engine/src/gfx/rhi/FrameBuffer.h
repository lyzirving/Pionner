#pragma once

#include "Texture.h"

namespace pio
{
	class RenderContext;

	struct FrameBufferSpecific
	{
		std::string Name;
		uint32_t Usage{ FrameBufferUsage_Color };
		uint32_t Width{ 0 }, Height{ 0 };

		std::vector<TextureSpecific> ColorSpec;
		std::vector<TextureSpecific> DepthSpec;		
	};

	#define PIO_FBO_ADD_USAGE(flags, usage)  (flags = (flags | (usage)))
	#define PIO_FBO_RMV_USAGE(flags, usage)  (flags = (flags & (~usage)))
	#define PIO_FBO_HAS_USAGE(flags, usage)  (flags & usage)

	#define PIO_FBO_IS_COLOR_BUF(flags)	PIO_FBO_HAS_USAGE(flags, FrameBufferUsage_Color)
	#define PIO_FBO_IS_DEPTH_BUF(flags)	PIO_FBO_HAS_USAGE(flags, FrameBufferUsage_Depth)
	#define PIO_FBO_IS_DEPTH_STENCIL(flags)	 (PIO_FBO_HAS_USAGE(flags, FrameBufferUsage_Depth) &&\
											  PIO_FBO_HAS_USAGE(flags, FrameBufferUsage_Stencil))	

	class FrameBuffer : public RenderResource
	{
		RTTR_ENABLE(RenderResource)
	public:
		FrameBuffer(const Ref<RenderContext>& context, const FrameBufferSpecific& spec);
		FrameBuffer(const Ref<RenderContext>& context, const std::string& name);
		virtual ~FrameBuffer() = default;

		virtual const std::vector<Ref<Texture>>& ColorBuffers() const = 0;
		virtual const std::vector<Ref<Texture>>& DepthBuffers() const = 0;
		virtual const Ref<Texture>& DepthBuffer() const = 0;
		virtual void BindWritingDepth(uint8_t sel = 0) = 0;		

		virtual const FrameBufferSpecific& Spec() const { return m_Spec; }

		uint32_t Width() const { return Spec().Width; }
		uint32_t Height() const { return Spec().Height; }

	protected:
		FrameBufferSpecific m_Spec;

	public:
		static Ref<FrameBuffer> Create(const Ref<RenderContext>& context, const FrameBufferSpecific& spec);
	};
}