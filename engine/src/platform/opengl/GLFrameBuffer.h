#ifndef __PIONNER_PLATFORM_OPENGL_GL_FRAME_BUFFER_H__
#define __PIONNER_PLATFORM_OPENGL_GL_FRAME_BUFFER_H__

#include "gfx/rhi/FrameBuffer.h"

namespace pio
{
	class GLFrameBuffer : public FrameBuffer
	{
	public:
		GLFrameBuffer(const FrameBufferSpecification &spec);
		virtual ~GLFrameBuffer();

		virtual bool init() override;
		virtual void destroy() override;

		virtual void bind() override;
		virtual void bindTarget(ColorAttachment attach, LightDir face, uint32_t mipLevel = 0) override;
		virtual void unbind() override;

		virtual Ref<Texture2D> getColorBuffer(ColorAttachment attach) const override { if ((uint8_t)attach < m_colorBuffers.size()) { return m_colorBuffers[(uint8_t)attach]; } else { return Ref<Texture2D>(); } }
		virtual Ref<Texture> getDepthBuffer() const override { return m_depthBuffer; }
		virtual Ref<Texture> getDepthStencilBuffer() const override { return m_depthBuffer; }

		virtual bool isInit() const override { return m_id != 0; }
		virtual uint32_t getId() const { return m_id; };
		virtual uint32_t getWidth() const override { return m_spec.Width; }
		virtual uint32_t getHeight() const override { return m_spec.Height; }
		virtual const FrameBufferSpecification &getSpecification() const override { return m_spec; }

	private:
		FrameBufferSpecification m_spec;
		uint32_t m_id{ 0 };

		std::vector<Ref<Texture2D>> m_colorBuffers;

		Ref<Texture> m_depthBuffer;
		DepthAttachment m_depthSlot{ DepthAttachment::Depth };
	};
}

#endif