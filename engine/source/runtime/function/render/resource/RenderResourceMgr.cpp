#include "function/render/resource/RenderResourceMgr.h"

#include "function/render/rhi/opengl/buffer/GLVertexBuffer.h"
#include "function/render/rhi/opengl/buffer/GLIndexBuffer.h"
#include "function/render/rhi/opengl/buffer/GLTetxure.h"

#include "function/render/rhi/opengl/buffer/GLDepthBuffer.h"

#include "core/log/LogSystem.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "RenderResourceMgr"

namespace Pionner
{
	RenderResourceMgr::RenderResourceMgr(const std::shared_ptr<Rhi> &rhi)
		: m_rhi(rhi)
		, m_vertexArray(rhi)
		, m_indiceArray(rhi)
		, m_textureArray(rhi)
		, m_frameBuffers(rhi)
	{
	}

	RenderResourceMgr::~RenderResourceMgr()
	{
		m_rhi.reset();
		m_weakSelf.reset();
	}

	std::shared_ptr<GfxBuffer> RenderResourceMgr::allocate(BufferType type)
	{
		std::shared_ptr<GfxBuffer> ret{ nullptr };
		std::shared_ptr<RenderResourceMgr> mgr = m_weakSelf.lock();
		if (mgr)
		{
			switch (type)
			{
				case BUF_VERTEX:
					ret = m_vertexArray.allocate(type, mgr);
					break;
				case BUF_INDICE:
					ret = m_indiceArray.allocate(type, mgr);
					break;
				case BUF_TEXTURE:
					ret = m_textureArray.allocate(type, mgr);
					break;
				default:
					break;
			}
		}
		return ret;
	}

	std::shared_ptr<GfxFrameBuffer> RenderResourceMgr::allocFbo(BufferType type)
	{
		std::shared_ptr<GfxFrameBuffer> ret{ nullptr };
		std::shared_ptr<RenderResourceMgr> mgr = m_weakSelf.lock();
		if (mgr)
		{
			ret = m_frameBuffers.allocate(type, mgr);
		}
		return ret;
	}

	std::shared_ptr<GfxBuffer> RenderResourceMgr::createHolderBuffer(BufferType type)
	{
		std::shared_ptr<GfxBuffer> result{ nullptr };
		std::shared_ptr<RenderResourceMgr> mgr = m_weakSelf.lock();
		if (mgr)
		{
			switch (type)
			{
				case Pionner::BUF_TEXTURE:
					result = std::shared_ptr<GfxBuffer>(new GLTexture(true, mgr));
					break;
				default:
					break;
			}
		}
		return result;
	}

	void RenderResourceMgr::checkAbandoned()
	{
		m_vertexArray.checkAbandoned();
		m_indiceArray.checkAbandoned();
		m_textureArray.checkAbandoned();
		m_frameBuffers.checkAbandoned();
	}

	void RenderResourceMgr::notifyRelease(BufferType type, uint32_t slot)
	{
		switch (type)
		{
			case BUF_VERTEX:
			{
				m_vertexArray.release(slot);
				break;
			}
			case BUF_INDICE:
			{
				m_indiceArray.release(slot);
				break;
			}
			case BUF_TEXTURE:
			{
				m_textureArray.release(slot);
				break;
			}
			case BUF_DEPTH:
			case BUF_CUBE_DEPTH:
			{
				m_frameBuffers.release(slot);
				break;
			}
			default:
				break;
		}
	}

	std::shared_ptr<GfxBuffer> RenderResourceMgr::find(BufferType type, uint32_t slot)
	{
		std::shared_ptr<GfxBuffer> ret{ nullptr };
		switch (type)
		{
			case BUF_VERTEX:
			{
				ret = m_vertexArray.find(slot);
				break;
			}
			case BUF_INDICE:
			{
				ret = m_indiceArray.find(slot);
				break;
			}
			case BUF_TEXTURE:
			{
				ret = m_textureArray.find(slot);
				break;
			}
			default:
				break;
		}
		return ret;
	}

	std::shared_ptr<GfxFrameBuffer> RenderResourceMgr::findFbo(uint32_t slot)
	{
		std::shared_ptr<GfxFrameBuffer> ret = m_frameBuffers.find(slot);
		return ret;
	}

	void RenderResourceMgr::shutdown()
	{
		m_vertexArray.clearActive();
		m_indiceArray.clearActive();
		m_textureArray.clearActive();
		m_frameBuffers.clearActive();
		checkAbandoned();
	}

	void RenderResourceMgr::makeSelfWeak(const std::shared_ptr<RenderResourceMgr> &self)
	{
		m_weakSelf = self;
	}

}