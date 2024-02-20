#ifndef __PIONNER_PLATFORM_OPENGL_GL_RENDER_BUFFER_H__
#define __PIONNER_PLATFORM_OPENGL_GL_RENDER_BUFFER_H__

#include "gfx/rhi/RenderBuffer.h"

namespace pio
{
	/*
	* Render buffers are write-only(can not be directly accessed in shader),
	* they are often used as depth and stencil attachments.
	*/
	class GLRenderBuffer : public RenderBuffer
	{
	public:
		GLRenderBuffer(const TextureSpecification &spec);
		virtual ~GLRenderBuffer();

		virtual bool init() override;
		virtual void destroy() override;

		virtual void bind() override;
		virtual void unbind() override;

		virtual bool isInit() const override { return m_bufferId != 0; }
		virtual ImageInternalFormat getFormat() const override { return m_spec.Format; }
		virtual uint32_t getWidth() const override { return m_spec.Width; }
		virtual uint32_t getHeight() const override { return m_spec.Height; }
		virtual uint32_t getSize() const override { return m_size; }

		virtual const std::string &getName() const override { return m_spec.Name; }
		virtual uint32_t getId() const override { return m_bufferId; }
		virtual bool SRGB() const override { return m_spec.SRGB; }

	private:
		uint32_t m_bufferId{ 0 };
		uint32_t m_size{ 0 };

		TextureSpecification m_spec{};
		TextureInternalFmt m_internalFmt{ TextureInternalFmt::None };
		PixelType m_pixelType{ PixelType::None };
	};
}

#endif