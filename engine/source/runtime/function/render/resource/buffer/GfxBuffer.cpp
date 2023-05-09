#include "function/render/resource/buffer/GfxBuffer.h"
#include "function/render/resource/RenderResourceMgr.h"

#include "core/log/LogSystem.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "GfxBuffer"

namespace Pionner
{
	Pionner::GfxBuffer::GfxBuffer(const std::shared_ptr<RenderResourceMgr> &mgr)
		: m_mgr(mgr), m_id(0), m_slot(0)
		, m_bufferType(BUF_CNT), m_dataType(DATA_TYPE_COUNT)
		, m_uploaded(false), m_abandoned(false)
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

	void GfxBuffer::bindTarget(uint32_t target)
	{
		//empty implementation by default
	}

	void GfxBuffer::unbind()
	{
		//empty implementation by default
	}

	void GfxBuffer::deleteResource()
	{
		//empty implementation by default
	}

	uint32_t GfxBuffer::size()
	{
		return 0;
	}

	void GfxBuffer::loadRawData()
	{
		//empty implementation by default
	}

	void GfxBuffer::notifyRelease()
	{
		if (isAbandonded())
			return;

		if (isCreated())
		{
			std::shared_ptr<RenderResourceMgr> mgr = m_mgr.lock();
			if (mgr)
			{
				mgr->release(m_bufferType, m_slot);
				m_abandoned = true;
			}
		}
	}
}