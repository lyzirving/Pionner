#ifndef __PIONNER_GL_DEPTH_FRAME_BUFFER_H__
#define __PIONNER_GL_DEPTH_FRAME_BUFFER_H__

#include "render/resource/buffer/GfxFrameBuffer.h"

namespace pio
{
	class GLDepthBuffer : public GfxFrameBuffer
	{
	public:
		GLDepthBuffer(const std::shared_ptr<RenderResourceMgr> &mgr);
		GLDepthBuffer(uint32_t width, uint32_t height, const std::shared_ptr<RenderResourceMgr> &mgr);
		virtual ~GLDepthBuffer();

		virtual void upload() override;

		virtual void bind() override;
		virtual void unbind() override;
		virtual void deleteResource() override;
	};

	template<>
	bool GfxBuffer::is<GLDepthBuffer>() const;

	template<>
	GLDepthBuffer *GfxBuffer::getPtr<GLDepthBuffer>();
}

#endif