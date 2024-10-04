#ifndef __PIONNER_SCENE_3D_SHADOW_MAP_FRAME_BUFFER_H__
#define __PIONNER_SCENE_3D_SHADOW_MAP_FRAME_BUFFER_H__

#include "gfx/rhi/FrameBufferProxy.h"

namespace pio
{
	class ShadowMapFrameBuffer : public FrameBufferProxy
	{
	public:
		ShadowMapFrameBuffer(Ref<RenderContext>& context);
		ShadowMapFrameBuffer(Ref<RenderContext>& context, uint32_t width, uint32_t height);
		~ShadowMapFrameBuffer() = default;

	private:
		void initInner(Ref<RenderContext>& context, uint32_t width, uint32_t height);
	};

	class CascadeShadowFrameBuffer : public FrameBufferProxy
	{
	public:
		CascadeShadowFrameBuffer(Ref<RenderContext>& context);
		CascadeShadowFrameBuffer(Ref<RenderContext>& context, uint32_t width, uint32_t height);
		~CascadeShadowFrameBuffer() = default;

	private:
		void initInner(Ref<RenderContext>& context, uint32_t width, uint32_t height);
	};
}

#endif