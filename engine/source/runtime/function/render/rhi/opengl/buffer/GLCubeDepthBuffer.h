#ifndef __PIONNER_GL_CUBE_DEPTH_BUFFER_H__
#define __PIONNER_GL_CUBE_DEPTH_BUFFER_H__

#include "GLDepthBuffer.h"

namespace Pionner
{
	class GLCubeDepthBuffer : public GLDepthBuffer
	{
	public:
		GLCubeDepthBuffer(const std::shared_ptr<RenderResourceMgr> &mgr);
		GLCubeDepthBuffer(uint32_t width, uint32_t height, const std::shared_ptr<RenderResourceMgr> &mgr);
		virtual ~GLCubeDepthBuffer();

		virtual void upload() override;
	};

	template<>
	bool GfxBuffer::is<GLCubeDepthBuffer>() const;

	template<>
	GLCubeDepthBuffer *GfxBuffer::getPtr<GLCubeDepthBuffer>();
}

#endif