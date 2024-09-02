#ifndef __PIONNER_GFX_RHI_OPENGL_GL_TEXTURE_2D_H__
#define __PIONNER_GFX_RHI_OPENGL_GL_TEXTURE_2D_H__

#include "gfx/rhi/Texture.h"

namespace pio
{
	class GLTexture2D : public Texture2D
	{
	public:
		GLTexture2D(Ref<RenderContext>& context, const TextureSpecific& spec) : Texture2D(context, spec) {}
		~GLTexture2D() = default;

		virtual bool init() override;
		virtual void destroy() override;
		virtual bool isInit() const { return m_id != 0; }

		virtual void bind() override;
		virtual void unbind() override;

		virtual uint32_t size() const override { return m_size; }
		virtual void setData(const void* data, uint32_t size, uint32_t offset = 0) override { /*do nothing*/ }

		virtual void active(TextureSampler sampler) override;

	protected:
		uint8_t* m_data{ nullptr };
	};
}

#endif