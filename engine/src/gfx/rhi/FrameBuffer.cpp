#include "FrameBuffer.h"

#include "gfx/renderer/RenderContext.h"
#include "gfx/rhi/opengl/GLFrameBuffer.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "FrameBuffer"

namespace pio
{
	FrameBuffer::FrameBuffer(const Ref<RenderContext>& context, const FrameBufferSpecific& spec)
		: RenderResource(context, RenderResourceType::FBO, spec.Name), m_Spec(spec)
	{
	}

	FrameBuffer::FrameBuffer(const Ref<RenderContext>& context, const std::string& name)
		: RenderResource(context, RenderResourceType::FBOProxy, name)
	{
		m_Spec.Name = name;
	}

	Ref<FrameBuffer> FrameBuffer::Create(const Ref<RenderContext>& context, const FrameBufferSpecific& spec)
	{
		switch (context->RenderBackend())
		{
		case RenderBackend_OpenGL:
			return CreateRef<GLFrameBuffer>(context, spec);
		default:
			LOGE("err! backend[%u] has not been implemented", context->RenderBackend());
			std::abort();
			return Ref<FrameBuffer>();
		}
	}
}