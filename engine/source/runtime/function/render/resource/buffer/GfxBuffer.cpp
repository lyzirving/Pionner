#include "function/render/resource/buffer/GfxBuffer.h"
#include "function/render/resource/RenderResourceMgr.h"

namespace Pionner
{
	Pionner::GfxBuffer::GfxBuffer(const std::shared_ptr<RenderResourceMgr> &mgr)
		: m_mgr(mgr), m_id(0), m_slot(0)
		, m_bufferType(BUF_CNT), m_dataType(DATA_TYPE_COUNT)
		, m_loaded(false)
	{
	}

	GfxBuffer::~GfxBuffer()
	{
		m_mgr.reset();
	}

	void GfxBuffer::bind()
	{
		//empty implementation by default
	}

	void GfxBuffer::bindToTarget(uint32_t target)
	{
		//empty implementation by default
	}
}