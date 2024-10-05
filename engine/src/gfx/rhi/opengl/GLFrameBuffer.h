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

		virtual const std::vector<Ref<Texture>>& colorBuffers() const override { return m_colorBuffs; }
		virtual const std::vector<Ref<Texture>>& depthBuffers() const override { return m_depthBuffs; }
		virtual const Ref<Texture>& depthBuffer() const override { return m_depthBuffs[m_depSel]; };

		virtual void bindWritingDepth(uint8_t sel = 0) override;

	protected:
		bool setDepBinding(uint8_t sel);

	private:
		std::vector<Ref<Texture>> m_colorBuffs;
		std::vector<Ref<Texture>> m_depthBuffs;
		uint8_t m_depSel{ 0 };
	};
}

#endif