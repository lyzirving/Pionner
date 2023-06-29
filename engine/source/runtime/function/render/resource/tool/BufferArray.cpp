#include "BufferArray.h"

#include "function/render/resource/RenderResourceMgr.h"
#include "function/render/rhi/Rhi.h"

#include "function/render/rhi/opengl/buffer/GLVertexBuffer.h"
#include "function/render/rhi/opengl/buffer/GLIndexBuffer.h"
#include "function/render/rhi/opengl/buffer/GLTexture.h"
#include "function/render/rhi/opengl/buffer/GLCubeTexture.h"

#include "function/render/rhi/opengl/buffer/GLDepthBuffer.h"
#include "function/render/rhi/opengl/buffer/GLCubeDepthBuffer.h"

namespace Pionner
{
	template<>
	std::shared_ptr<GfxBuffer> BufferArray<GfxBuffer>::createBuffer(BufferType type, std::shared_ptr<RenderResourceMgr> &mgr)
	{
		std::shared_ptr<GfxBuffer> ret{ nullptr };
		switch (type)
		{
			case BUF_VERTEX:
				ret = std::shared_ptr<GfxBuffer>(new GLVertexBuffer(mgr));
				break;
			case BUF_INDICE:
				ret = std::shared_ptr<GfxBuffer>(new GLIndexBuffer(mgr));
				break;
			case BUF_TEXTURE:
				ret = std::shared_ptr<GfxBuffer>(new GLTexture(mgr));
				break;
			case BUF_CUBE_TEXTURE:
				ret = std::shared_ptr<GfxBuffer>(new GLCubeTexture(mgr));
				break;
			default:
				break;
		}
		return ret;
	}

	template<>
	std::shared_ptr<GfxFrameBuffer> BufferArray<GfxFrameBuffer>::createBuffer(BufferType type, std::shared_ptr<RenderResourceMgr> &mgr)
	{
		std::shared_ptr<GfxFrameBuffer> ret{ nullptr };
		switch (type)
		{
			case BUF_DEPTH_FBO:
				ret = std::shared_ptr<GfxFrameBuffer>(new GLDepthBuffer(mgr));
				break;
			case BUF_CUBE_DEPTH_FBO:
				ret = std::shared_ptr<GfxFrameBuffer>(new GLCubeDepthBuffer(mgr));
				break;
			default:
				break;
		}
		return ret;
	}

}