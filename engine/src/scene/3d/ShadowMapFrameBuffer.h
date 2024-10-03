#ifndef __PIONNER_SCENE_3D_SHADOW_MAP_FRAME_BUFFER_H__
#define __PIONNER_SCENE_3D_SHADOW_MAP_FRAME_BUFFER_H__

#include "gfx/rhi/FrameBuffer.h"

namespace pio
{
	class CascadeShadowFrameBuffer : public FrameBuffer
	{
	public:
		CascadeShadowFrameBuffer(Ref<RenderContext>& context);
		CascadeShadowFrameBuffer(Ref<RenderContext>& context, uint32_t width, uint32_t height);
		~CascadeShadowFrameBuffer() = default;

		virtual bool init() override { return m_frameBuff->init(); }
		virtual void destroy() override { m_frameBuff->destroy(); }
		virtual bool isInit() const { return m_frameBuff->isInit(); }

		virtual void bind() override { m_frameBuff->bind(); }
		virtual void unbind() override { m_frameBuff->unbind(); }

		virtual const std::vector<Ref<Texture>>& colorBuffers() const override { return m_frameBuff->colorBuffers(); }
		virtual const Ref<Texture>& depthBuffer() const override { return m_frameBuff->depthBuffer(); };

		virtual void bindWritingDepth(uint8_t sel = 0) override { m_frameBuff->bindWritingDepth(sel); };

	private:
		void initInner(Ref<RenderContext>& context, uint32_t width, uint32_t height);

	private:
		Ref<FrameBuffer> m_frameBuff;
	};
}

#endif