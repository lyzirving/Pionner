#ifndef __PIONNER_GFX_RHI_OPENGL_GL_RENDER_BUFFER_H__
#define __PIONNER_GFX_RHI_OPENGL_GL_RENDER_BUFFER_H__

#include "gfx/rhi/Texture.h"

namespace pio
{
	class GLRenderBuffer : public RenderBuffer
	{
	public:
		GLRenderBuffer(Ref<RenderContext>& context, const TextureSpecific& spec);
		~GLRenderBuffer() = default;

		virtual bool init() override;
		virtual void destroy() override;
		virtual bool isInit() const { return m_id != 0; }

		virtual void bind() override;
		virtual void bind(uint32_t width, uint32_t height) override;
		virtual void unbind() override;

		virtual uint32_t size() const override { return m_size; }
		virtual void setData(const void* data, uint32_t size, uint32_t offset = 0) override { /*do nothing*/ }
	};
}

#endif