#ifndef __PIONNER_GFX_RHI_OPENGL_GL_FRAME_BUFFER_H__
#define __PIONNER_GFX_RHI_OPENGL_GL_FRAME_BUFFER_H__

#include "gfx/rhi/FrameBuffer.h"

namespace pio
{
	class GLFrameBuffer : public FrameBuffer
	{
	public:
		GLFrameBuffer(Ref<RenderContext>& context, const FrameBufferSpecific& spec);
		~GLFrameBuffer() = default;

		virtual bool init() override;
		virtual void destroy() override;
		virtual bool isInit() const { return m_id != 0; }

		virtual void bind() override;
		virtual void unbind() override;

		virtual uint32_t size() const override { return 0; }
		virtual void setData(const void* data, uint32_t size, uint32_t offset = 0) override { /*do nothing*/ }

	private:
		std::vector<Ref<Texture>> m_colorBuffs;
		Ref<Texture> m_depthBuff;
	};
}

#endif