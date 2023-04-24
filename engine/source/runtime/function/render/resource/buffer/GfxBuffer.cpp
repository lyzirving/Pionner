#include "function/render/resource/buffer/GfxBuffer.h"
#include "function/render/resource/RenderResourceMgr.h"

namespace Pionner
{
	Pionner::GfxBuffer::GfxBuffer(const std::shared_ptr<RenderResourceMgr> &mgr)
		: m_mgr(mgr), m_id(0), m_bufferType(BUF_CNT), m_dataType(DATA_TYPE_COUNT), m_loaded(false)
	{
	}

	GfxBuffer::~GfxBuffer()
	{
		m_mgr.reset();
	}

	void GfxBuffer::insertData(uint32_t size, void *data)
	{
	}

	void GfxBuffer::bindToTarget(uint32_t target)
	{
	}
}