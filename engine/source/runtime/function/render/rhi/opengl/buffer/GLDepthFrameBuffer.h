#ifndef __PIONNER_GL_DEPTH_FRAME_BUFFER_H__
#define __PIONNER_GL_DEPTH_FRAME_BUFFER_H__

#include "function/render/resource/buffer/GfxFrameBuffer.h"

namespace Pionner
{
	class GLDepthFrameBuffer : public GfxFrameBuffer
	{
	public:
		GLDepthFrameBuffer(const std::shared_ptr<RenderResourceMgr> &mgr);
		GLDepthFrameBuffer(uint32_t width, uint32_t height, const std::shared_ptr<RenderResourceMgr> &mgr);
		virtual ~GLDepthFrameBuffer();

		virtual void upload() override;

		virtual void bind() override;
		virtual void unbind() override;
		virtual void deleteResource() override;
	};

	template<>
	bool GfxBuffer::is<GLDepthFrameBuffer>() const;

	template<>
	GLDepthFrameBuffer *GfxBuffer::getPtr<GLDepthFrameBuffer>();
}

#endif