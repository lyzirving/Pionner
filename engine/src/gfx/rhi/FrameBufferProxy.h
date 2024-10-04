#ifndef __PIONNER_GFX_RHI_FRAME_BUFFER_PROXY_H__
#define __PIONNER_GFX_RHI_FRAME_BUFFER_PROXY_H__

#include "FrameBuffer.h"

namespace pio
{
	class FrameBufferProxy : public FrameBuffer
	{
	public:
		FrameBufferProxy(Ref<RenderContext>& context, const std::string& name);
		FrameBufferProxy(Ref<RenderContext>& context, const std::string& name, uint32_t width, uint32_t height);
		virtual ~FrameBufferProxy() = default;

		virtual bool init() override { return m_frameBuff->init(); }
		virtual void destroy() override { m_frameBuff->destroy(); }
		virtual bool isInit() const { return m_frameBuff->isInit(); }

		virtual void bind() override { m_frameBuff->bind(); }
		virtual void unbind() override { m_frameBuff->unbind(); }

		virtual const FrameBufferSpecific& spec() const override { return m_frameBuff->spec(); }

		virtual const std::vector<Ref<Texture>>& colorBuffers() const override { return m_frameBuff->colorBuffers(); }
		virtual const Ref<Texture>& depthBuffer() const override { return m_frameBuff->depthBuffer(); };

		virtual void bindWritingDepth(uint8_t sel = 0) override { m_frameBuff->bindWritingDepth(sel); };

	protected:
		Ref<FrameBuffer> m_frameBuff;
	};
}

#endif