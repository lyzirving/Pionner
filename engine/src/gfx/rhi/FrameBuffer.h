#ifndef __PIONNER_GFX_RHI_FRAME_BUFFER_H__
#define __PIONNER_GFX_RHI_FRAME_BUFFER_H__

#include "Texture.h"
#include "gfx/renderer/RenderData.h"

namespace pio
{
	struct FrameBufferSpecification
	{
		std::string Name;
		uint32_t Width{ 0 }, Height{ 0 };
		bool Mipmap{ false };

		FrameBufferUsage FrameBufferUsage{ FrameBufferUsage::ColorBuffer };
		std::vector<TextureSpecification> ColorBufferSpec;
		TextureSpecification DepthBufferSpec;
		DepthAttachment DepthAttachment{ DepthAttachment::Depth };

		float DepthClearVal{ 0.0f };
		glm::vec4 ClearColor{ 0.0f, 0.0f, 0.0f, 1.0f };
	};

	class FrameBuffer
	{
	public:
		virtual ~FrameBuffer() = default;

		virtual bool init() = 0;
		virtual void destroy() = 0;

		virtual void bind() = 0;
		// For Framebuffer who has a cube texture as rendering target
		virtual void bindTarget(ColorAttachment attach, LightDir face, uint32_t mipLevel = 0) = 0;
		virtual void unbind() = 0;		

		virtual bool isInit() const = 0;
		virtual uint32_t getId() const = 0;

		virtual Ref<Texture2D> getColorBuffer(ColorAttachment attach) const = 0;
		virtual Ref<Texture> getDepthBuffer() const = 0;
		virtual Ref<Texture> getDepthStencilBuffer() const = 0;

		virtual uint32_t getWidth() const = 0;
		virtual uint32_t getHeight() const = 0;
		virtual const FrameBufferSpecification &getSpecification() const = 0;

	public:
		static Ref<FrameBuffer> Create(const FrameBufferSpecification &spec);
		static bool IsColorBuffer(FrameBufferUsage fmt) { return fmt == FrameBufferUsage::ColorBuffer; };
		static bool IsDepthBuffer(FrameBufferUsage fmt) { return fmt == FrameBufferUsage::DepthBuffer; };
		static bool IsDepthStencil(FrameBufferUsage fmt) { return fmt == FrameBufferUsage::DepthStencil; }
	};
}

#endif